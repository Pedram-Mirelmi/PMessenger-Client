#include "DataBase.hpp"
#include "../../Commons/KeyWords.hpp"
#include "../Others/format.hpp"


static char database_filename[] = "db.sqlite";
static char data_path[] = "/home/pedram/Desktop/PMessenger/Client/data";

static char users_table_creation[] =
        "CREATE TABLE IF NOT EXISTS users"
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
        "    message_text        TEXT NOT NULL ,"
        "    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL ,"
        "    FOREIGN KEY (owner_id) REFERENCES users(user_id),"
        "    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id)"
        ");";


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
    return
            create_table.exec(users_table_creation) &&
            create_table.exec(chat_envs_table_creation) &&
            create_table.exec(private_chats_table_creation) &&
            create_table.exec(messages_table_creation);
}

bool DataBase::insertPrivateEnv(const QJsonObject &env)
{
    using namespace KeyWords;
    auto general_env_insert_Qry = string_format("INSERT INTO chat_envs(env_id, participates) VALUES(%d, 1);", env[ENV_ID].toInt());
    auto private_env_insert_Qry = string_format("INSERT INTO private_chats(env_id, first_person, second_person) VALUES(%d, %d, %d);",
                                                env[ENV_ID].toInt(), env[FIRST_PERSON].toInt(), env[SECOND_PERSON].toInt());
    this->execOtherQry(general_env_insert_Qry.c_str());
    return this->execOtherQry(private_env_insert_Qry.c_str());
}

// public
bool DataBase::insertGroupEnv(const QJsonObject &env)
{
    // TODO
}

// public
bool DataBase::insertChannelEnv(const QJsonObject &env)
{
    // TODO
}

// public
void DataBase::insertGroupMessages(const QJsonArray &messages)
{
    // TODO
}

// public
void DataBase::insertChannelMessages(const QJsonArray &messages)
{
    // TODO
}



// public
void DataBase::insertPrivateMessages(const QJsonArray &messages)
{
    using namespace KeyWords;
    for (const auto& message : this->convertToNormalForm(messages))
    {
        this->insertSinglePrivateMessage(message.toObject());
    }
}

// private
void DataBase::insertSinglePrivateMessage(const QJsonObject& msg_info)
{
    using namespace KeyWords;
    bool ok;
    auto general_message_insert_query = string_format(
                "INSERT INTO messages(message_id, owner_id, env_id, created_at) VALUES "
                "                     (%d,        %d,       %d,     '%s' )",
                msg_info[MESSAGE_ID].toInt(), msg_info[OWNER_ID].toInt(), msg_info[ENV_ID].toInt(), msg_info[CREATED_AT].toString());
    ok = this->execOtherQry(general_message_insert_query.c_str());
    auto text_message_insert_query = string_format(
                    "INSERT INTO text_messages(message_id, message_text) VALUES "
                    "                          (%d          &s)",
                                               msg_info[MESSAGE_ID].toInt(), msg_info[MESSAGE_TEXT].toString());
    ok = this->execOtherQry(text_message_insert_query.c_str()) && ok;
    if (msg_info.contains(SEEN) && !msg_info[SEEN].toBool() && ok)
        emit this->newMessageInserted(msg_info);
}

// private
bool DataBase::SELECT(QJsonArray &result_set, char query_str[]) const
{
    QSqlQuery query(this->db);
    bool ok = query.exec(query_str);
    while (query.next())
    {
        QJsonObject record;
        for (quint8 i = 0; i < query.record().count(); i++)
        {
            record[query.record().field(i).name()] = query.value(i).toString();
        }
        result_set.append(record);
    }
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


