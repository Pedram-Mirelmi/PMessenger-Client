#include "DataBase.hpp"
#include "../../Commons/KeyWords.hpp"
#include <string>
#include <fmt/format.h>

static char database_filename[] = "db.sqlite";
static char data_path[] = "/home/pedram/Desktop/PMessenger/Client/data";

static char contacts_table_creation[] =
        "CREATE TABLE IF NOT EXISTS contacts"
        "("
        "   user_id             INTEGER PRIMARY KEY,"
        "   username            VARCHAR(63) NOT NULL UNIQUE ,"
        "   name                VARCHAR(63) NOT NULL ,"
        "   name_saved          VARCHAR(63) NOT NULL"
        ");";


static char chat_envs_table_creation[] =
        "CREATE TABLE IF NOT EXISTS chat_envs"
        "("
        "    env_id              INTEGER PRIMARY KEY,"
        "    participates        INTEGER NOT NULL"
        ");";


static char private_chats_table_creation[] =
        "CREATE TABLE IF NOT EXISTS private_chats"
        "("
        "    env_id              INTEGER PRIMARY KEY,"
        "    first_person        INTEGER NOT NULL ,"
        "    second_person       INTEGER NOT NULL ,"
        "    UNIQUE (first_person, second_person),"
        "    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id),"
        "    FOREIGN KEY (first_person) REFERENCES users(user_id),"
        "    FOREIGN KEY (second_person) REFERENCES users(user_id)"
        ");";


static char messages_table_creation[] =
        "CREATE TABLE IF NOT EXISTS messages"
        "("
        "    message_id          INTEGER PRIMARY KEY,"
        "    owner_id            INTEGER NOT NULL ,"
        "    env_id              INTEGER NOT NULL ,"
        "    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL ,"
        "    FOREIGN KEY (owner_id) REFERENCES users(user_id),"
        "    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id)"
        ");";

static char text_messages_table_creation[] =
        "CREATE TABLE text_messages"
        "("
        "    message_id          INTEGER UNSIGNED PRIMARY KEY ,"
        "    message_text        TEXT NOT NULL,"
        "    FOREIGN KEY (message_id) REFERENCES messages(message_id)"
        ");";

static char text_messages_view_creatoin[] =
        "CREATE VIEW text_messages_view AS"
        "    SELECT"
        "        m.message_id, "
        "        m.owner_id, "
        "        m.env_id, "
        "        m.created_at, "
        "        tm.message_text "
        "   FROM messages m "
        "   INNER JOIN text_messages tm "
        "        ON m.message_id = tm.message_id; ";

DataBase::DataBase(QObject* parent)
    :QObject(parent), db(QSqlDatabase::addDatabase("QSQLITE"))
{

}

bool DataBase::tryToInit()
{
    QDir dir;
    if(!dir.exists(data_path))
    {
        if (dir.mkpath(data_path))
            qCritical() << "Couldn't create folder." << data_path;
        if (!this->tryToCreateDBFile())
            qCritical() << "Couldn't create database files";
        if (!this->db.open())
            qCritical() << "Couldn't open database";
        if (!this->createTables())
            qCritical() << "Couldn't create tables";
        return true;
    }
    QFile file;
    if (!file.exists(QString(data_path) + database_filename))
    {
        if (!this->tryToCreateDBFile())
            qCritical() << "Couldn't create database files";
        if (!this->db.open())
            qCritical() << "Couldn't open database";
        if (!this->createTables())
            qCritical() << "Couldn't create tables";
        return true;
    }
    return false;
}

bool DataBase::tryToCreateDBFile()
{
    QFile db_file(database_filename);
    bool ok = db_file.open(QFile::ReadWrite);
    this->db.setDatabaseName(QString(data_path) + database_filename);
    return ok;
}

bool DataBase::createTables()
{
    QSqlQuery create_table(this->db);
    return  create_table.exec(contacts_table_creation) &&
            create_table.exec(chat_envs_table_creation) &&
            create_table.exec(private_chats_table_creation) &&
            create_table.exec(messages_table_creation) &&
            create_table.exec(text_messages_table_creation) &&
            create_table.exec(text_messages_view_creatoin);
}

void DataBase::tryToInsertPrivateEnvs(const QJsonValueRef &envs)
{
    using namespace KeyWords;
    for (const auto &private_env_variant : envs.toArray())
        if (!envExists(private_env_variant.toInteger()))
            emit this->needPrivateEnvDetails(private_env_variant.toInteger());
}

void DataBase::insertPrivateEnvs(const QJsonValueRef &envs)
{
    for (const auto& env_info : envs.toArray())
        insertSinglePrivateEnv(env_info.toObject());
}

void DataBase::insertSinglePrivateEnv(const QJsonObject &env_info)
{
    using namespace KeyWords;
    const auto create_env_query = fmt::format("INSERT INTO chat_envs(env_id, participates) "
                                              "VALUES({}, 1);",
                                              env_info[ENV_ID].toInteger());
    this->execOtherQry(create_env_query.c_str());
    const auto create_private_query = fmt::format("INSERT INTO private_chats(env_id, first_person, second_person) "
                                                  "VALUES({},   {},     {});",
                                                  env_info[ENV_ID].toInteger(),
                                                  env_info[FIRST_PERSON].toInteger(),
                                                  env_info[SECOND_PERSON].toInteger());
}

// public
bool DataBase::insertGroupEnv(const QJsonObject &env)
{
    // TODO
    env.size();
    return true;
}

// public
bool DataBase::insertChannelEnv(const QJsonObject &env)
{
    // TODO
    env.size();
    return true;
}

// public
void DataBase::insertGroupMessages(const QJsonArray &messages)
{
    // TODO
    messages.size();
}

// public
void DataBase::insertChannelMessages(const QJsonArray &messages)
{
    // TODO
    messages.size();
}


// public
void DataBase::insertTextMessages(const QJsonArray &messages)
{
    using namespace KeyWords;
    for (const auto& message : this->convertToNormalForm(messages))
        this->insertSingleTextMessage(message.toObject());
}

// private
void DataBase::insertSingleTextMessage(const QJsonObject& msg_info)
{
    using namespace KeyWords;
    bool ok;
    auto general_message_insert_query = fmt::format(
                "INSERT INTO messages(message_id, owner_id, env_id, created_at) VALUES "
                "                     ({},        {},       {},     '{}' )",
                msg_info[MESSAGE_ID].toInt(), msg_info[OWNER_ID].toInt(),
                msg_info[ENV_ID].toInt(), msg_info[CREATED_AT].toString().toStdString());
    ok = this->execOtherQry(general_message_insert_query.c_str());
    auto text_message_insert_query = fmt::format(
                    "INSERT INTO text_messages(message_id, message_text) "
                    "VALUES ({},          '{}')",
                    msg_info[MESSAGE_ID].toInt(), toRaw(msg_info[MESSAGE_TEXT].toString().toStdString()));
    ok = this->execOtherQry(text_message_insert_query.c_str()) && ok;
    if (msg_info.contains(SEEN) && !msg_info[SEEN].toBool() && ok)
        emit this->newTextMessageInserted(msg_info);
}

// private
bool DataBase::SELECT(QVector<InfoContainer> &result_set, const char query_str[]) const
{
    QSqlQuery query(this->db);
    bool ok = query.exec(query_str);
    result_set.clear();
    result_set.resize(query.size());
    auto index = 0;
    while (query.next())
        for (quint8 i = 0; i < query.record().count(); i++)
            result_set[index++][query.record().field(i).name().toStdString().data()] = query.value(i).toString();

    return ok;
}

// private
bool DataBase::execOtherQry(const char query_str[])
{
    QSqlQuery query(this->db);
    return query.exec(query_str);
}

QJsonArray DataBase::convertToNormalForm(const QJsonArray &data)
{
    QJsonArray result;
    auto fields = data[0].toArray();
    for (auto itter = data.constBegin() + 1; itter < data.constEnd(); itter++)
    {
        QJsonObject new_row;
        const QJsonArray& data_row = itter->toArray();
        for (uint8_t i = 0; i<fields.size(); i++)
        {
            new_row[fields[i].toString()] = data_row[i];
        }
    }
    return result;
}

bool DataBase::envExists(const quint64 &env_id) const
{
    QVector<QHash<const char*, QVariant>> arr;
    this->SELECT(arr, fmt::format("SELECT * FROM chat_envs WHERE env_id={}", (uint64_t)env_id).c_str());
    return arr.isEmpty();
}

void DataBase::convertToHash(InfoContainer &target, const QJsonObject &source)
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}
