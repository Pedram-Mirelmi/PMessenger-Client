#include "DataBase.hpp"
#include "../../Commons/KeyWords.hpp"
//#include <string>
//#include <fmt/format.h>
#include <QMutex>


static char data_path[] = "/home/pedram/Desktop/PMessenger/Client/data/";
static char database_filename[] = "db.sqlite";
static char sql_codes_filename[] = "SqliteQueries.sql";

QMutex insert_lock; // to get last_insert_rowid() thead safly

DataBase::DataBase(QObject* parent, InfoContainer& user_info)
    :QObject(parent),
     db(QSqlDatabase::addDatabase("QSQLITE")),
     m_user_info(user_info)
{}

bool DataBase::tryToInit()
{
    qDebug() << this->db.lastError().text();
    QFile file;
    if (!file.exists(QString(data_path) + database_filename))
    {
        if (!this->tryToCreateDBFile())
            qCritical() << "Couldn't create or open database files";
        this->db.setDatabaseName(QString(data_path) + database_filename);
        if (!this->db.open())
            qCritical() << "Couldn't open database";
        if (!this->createTables())
            qCritical() << "Couldn't create tables";
        return true;
    }
    this->db.setDatabaseName(QString(data_path) + database_filename);
    if (!this->db.open())
        qCritical() << "Couldn't open database";

    return false; // did not inited db (existed before)
}

bool DataBase::tryToCreateDBFile()
{
    QFile db_file(QString(data_path) + database_filename);
    return db_file.open(QFile::ReadWrite);
}

bool DataBase::createTables()
{
    QSqlQuery create_table(this->db);
    QFile sql_codes_file(QString(data_path)+sql_codes_filename);
    sql_codes_file.open(QFile::ReadOnly);
    QTextStream stream(&sql_codes_file);
    auto all_queries = stream.readAll();
    if (!create_table.exec(all_queries))
        qDebug() << create_table.lastError().text();
    return true;
}

void DataBase::tryToInsertPrivateEnvs(const QJsonValueRef &envs)
{
    using namespace KeyWords;
    auto private_envs = envs.toArray();
    for (const auto &private_env_variant : private_envs)
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
                                              env_info[ENV_ID].toString().toULongLong());
    this->execOtherQry(create_env_query.c_str());
    const auto create_private_query = fmt::format("INSERT INTO "
                                                  "private_chats(env_id, first_person, second_person) "
                                                  "VALUES({},   {},     {});",
                                                  env_info[ENV_ID].toInteger(),
                                                  env_info[FIRST_PERSON].toInteger(),
                                                  env_info[SECOND_PERSON].toInteger());
    this->execOtherQry(create_private_query.c_str());
}

bool DataBase::privateChatAlreadyExists(const quint64 &user_id) const
{
    InfoContainer result;
    this->getPrivateChatInfo(user_id, result);
    return !result.isEmpty();
}

void DataBase::getPendingPrivateChatInfo(const quint64 &user_id,
                                         InfoContainer &chat_info) const
{
    this->singleSELECT(chat_info, fmt::format("SELECT * FROM pending_chat_envs "
                                              "WHERE first_person = {} OR second_person = {}",
                                              user_id, user_id).c_str());
}

void DataBase::getPrivateChatInfo(const quint64 &user_id,
                                  InfoContainer& chat_info) const
{
    this->singleSELECT(chat_info, fmt::format("SELECT * FROM private_chats "
                                              "WHERE first_person = {} OR second_person = {}",
                                              user_id, user_id).c_str());
}

void DataBase::getEnvTextMessages(const quint64 &env_id,
                                  QVector<InfoContainer> &messages,
                                  const bool& pending_env) const
{
    if (pending_env)
    {
        this->SELECT(messages, fmt::format("SELECT * FROM pending_messages "
                                           "WHERE invalid_env_id = {};",
                                            env_id).c_str());
        return;
    }
    this->SELECT(messages, fmt::format("SELECT * FROM text_messages_view "
                                       "WHERE env_id={};", env_id).c_str());
    this->SELECT(messages, fmt::format("SELECT * FROM pending_messages "
                                       "WHERE env_id={};", env_id).c_str());
}

void DataBase::getAllRegisteredEnvs(QVector<InfoContainer> &envs)
{
    this->SELECT(envs, "SELECT * FROM private_chats");
}

void DataBase::getAllPendingEnvs(QVector<InfoContainer> &envs)
{
    this->SELECT(envs, "SELECT * FROM pending_chat_envs");
}

QString DataBase::getNameOfUser(const quint64 &user_id)
{
    InfoContainer user_info;
    this->singleSELECT(user_info, fmt::format("SELECT name FROM users "
                                              "WHERE user_id = {}", user_id).c_str());
    return user_info[KeyWords::NAME].toString();
}

quint64 DataBase::getLastEnvMessageId(const quint64 env_id)
{
    InfoContainer info;
    this->singleSELECT(info, fmt::format("SELECT MAX(message_id) AS id "
                                         "FROM messages "
                                         "WHERE env_id = {};", env_id).c_str());
    return info["id"].toUInt();
}


// public
void DataBase::insertValidTextMessages(const QJsonArray &messages)
{
    using namespace KeyWords;
    for (const auto& message : this->convertToNormalForm(messages))
        this->insertValidTextMessage(message.toObject());
}

quint16 DataBase::insertNewPendingPrivateChat(const quint64 &user_id)
{
    using namespace KeyWords;
    insert_lock.lock();
    this->execOtherQry(fmt::format("INSERT INTO pending_chat_envs(env_type, first_person, second_person) "
                       "VALUES('private_chat', {}, {});", this->m_user_info[USER_ID].toUInt(), user_id).c_str());
    auto inserted_invalid_id = this->getLastInsertId();
    insert_lock.unlock();
    return inserted_invalid_id;
}

quint16 DataBase::insertPendingTextMessage(const quint16 &env_id,
                                             const QString &message_text,
                                             const bool& to_pending_env)
{
    insert_lock.lock();
    auto id_field = to_pending_env ? "invalid_env_id" : "env_id";
    this->execOtherQry(fmt::format("INSERT INTO pending_messages({}, message_text) "
                                   "VALUES({}, '{}');",
                                   id_field, env_id, toRaw(message_text.toStdString())).c_str());
    auto inserted_id = getLastInsertId();
    insert_lock.unlock();
    return inserted_id;
}

void DataBase::tryToInsertUser(const quint64 &user_id,
                               const QString &username,
                               const QString &name)
{
    this->execOtherQry(fmt::format("INSERT INTO users(user_id, username, name) "
                                   "VALUES({}, {}, {});",
                                   user_id, username.toStdString(), name.toStdString()).c_str());
}

void DataBase::deletePendingChat(const quint64 &invalid_id)
{
    this->execOtherQry(fmt::format("DELETE FROM pending_chat_envs WHERE invalid_env_id={}", invalid_id).c_str());
}

// private
void DataBase::insertValidTextMessage(const QJsonObject& msg_info)
{
    using namespace KeyWords;
    insert_lock.lock();
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
    insert_lock.unlock();
}

// private
bool DataBase::SELECT(QVector<InfoContainer> &result_set,
                      const char query_str[]) const
{
    QSqlQuery query(this->db);
    bool ok = query.exec(query_str);
    auto index = result_set.size();
    result_set.reserve(result_set.size() + query.size());
    while (query.next())
    {
        result_set.emplace_back();
        for (quint8 i = 0; i < query.record().count(); i++)
            result_set[index][query.record().field(i).name()] = query.value(i);
        index++;
    }
    return ok;
}

// private
bool DataBase::singleSELECT(InfoContainer &result,
                            const char query_str[]) const
{
    QSqlQuery query(this->db);
    bool ok = query.exec(query_str);
    result.clear();
    if (query.next())
        for (quint8 i = 0; i < query.record().count(); i++)
            result[query.record().field(i).name()] = query.value(i).toString();
    return ok;
}

// private
bool DataBase::execOtherQry(const char query_str[])
{
    QSqlQuery query(this->db);
    return query.exec(query_str);
}

// private
QJsonArray DataBase::convertToNormalForm(const QJsonArray &data) const
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

// private
bool DataBase::envExists(const quint64 &env_id) const
{
    QVector<InfoContainer> arr;
    this->SELECT(arr, fmt::format("SELECT * FROM chat_envs WHERE env_id={}", (uint64_t)env_id).c_str());
    return arr.isEmpty();
}

// private
quint64 DataBase::getLastInsertId() const
{
    InfoContainer select_result;
    this->singleSELECT(select_result, "SELECT last_insert_rowid() as id;");
    return select_result["id"].toUInt();
}

// private
quint64 DataBase::getMaxMessagesId() const
{
    InfoContainer select_result;
    this->singleSELECT(select_result, "SELECT max(message_id) FROM messages as id;");
    return select_result["id"].toUInt();
}

// private
void DataBase::convertToHash(InfoContainer &target,
                             const QJsonObject &source)
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}
