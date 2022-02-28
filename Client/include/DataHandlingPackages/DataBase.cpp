#include "DataBase.hpp"
#include "../../Commons/KeyWords.hpp"
#include <QMutex>
#include <memory>

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
    using namespace KeyWords;
    QFile file;
    if (!file.exists(QString(data_path) + this->m_user_info[USERNAME].toString() + ".sqlite"))
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
    this->db.setDatabaseName(QString(data_path) + this->m_user_info[USERNAME].toString() + ".sqlite");
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
                                                  env_info[ENV_ID].toString().toULongLong(),
                                                  env_info[FIRST_PERSON].toString().toULongLong(),
                                                  env_info[SECOND_PERSON].toString().toULongLong());
    this->execOtherQry(create_private_query.c_str());
}


bool DataBase::setValidPrivateChatInfoIfExists(const quint64 &user_id,
                                          InfoContainer& user_info) const
{

    auto info = this->getValidPrivateChatInfoByOtherUser(user_id);
    if (info->isEmpty())
        return false;
    user_info = *info;
    return true;
}

bool DataBase::setInvalidPrivateChatInfoIfExists(const quint64 &user_id,
                                                 InfoContainer& user_info) const
{
    auto info = this->getPendingPrivateChatInfoByOtherUser(user_id);
    if(info->isEmpty())
        return false;
    user_info = *info;
    return true;
}

std::shared_ptr<InfoContainer>
DataBase::getPendingPrivateChatInfoByOtherUser(const quint64 &user_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM pending_chat_envs "
                                          "WHERE first_person = {} OR second_person = {}",
                                          user_id, user_id).c_str());
}

std::shared_ptr<InfoContainer>
DataBase::getPendingPrivateChatInfoByEnvId(const quint64 &invalid_env_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM pending_chat_envs "
                                          "WHERE invalid_env_id = {}",
                                          invalid_env_id).c_str());
}

std::shared_ptr<InfoContainer>
DataBase::getValidPrivateChatInfoByOtherUser(const quint64 &user_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM private_chats "
                                          "WHERE first_person = {} OR second_person = {}",
                                          user_id, user_id).c_str());
}

std::shared_ptr<InfoContainer>
DataBase::getValidPrivateChatInfoByEnvId(const quint64 &env_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM private_chats "
                                          "WHERE env_id = {}",
                                          env_id).c_str());
}

std::shared_ptr<InfoCollection>
DataBase::getEnvTextMessages(const quint64 &env_id,
                             const bool& pending_env) const
{
    auto result = std::make_shared<InfoCollection>();
    if (pending_env)
    {
        return this->SELECT(fmt::format("SELECT * FROM pending_messages "
                                        "WHERE invalid_env_id = {};",
                                        env_id).c_str());
    }
    auto valids = this->SELECT(fmt::format("SELECT * FROM text_messages_view "
                                           "WHERE env_id={};", env_id).c_str());
    auto invalids = this->SELECT(fmt::format("SELECT * FROM pending_messages "
                                             "WHERE env_id={};", env_id).c_str());
    valids->append(*invalids);
    return valids;
}

std::shared_ptr<InfoCollection>
DataBase::getAllRegisteredEnvs()
{
    return this->SELECT("SELECT * FROM private_chats");
}

std::shared_ptr<InfoCollection>
DataBase::getAllPendingEnvs()
{
    return this->SELECT("SELECT * FROM pending_chat_envs");
}

QString DataBase::getNameOfUser(const quint64 &user_id) const
{
    auto user_info = this->singleSELECT(fmt::format("SELECT name FROM users "
                                                    "WHERE user_id = {}", user_id).c_str());
    return user_info->value(KeyWords::NAME).toString();
}

quint64 DataBase::getLastEnvMessageId(const quint64 env_id) const
{
    auto info = this->singleSELECT(fmt::format("SELECT MAX(message_id) AS id "
                                               "FROM messages "
                                               "WHERE env_id = {};", env_id).c_str());
    return info->value("id").toUInt();
}


// public
void DataBase::insertValidTextMessages(const QJsonArray &messages)
{
    using namespace KeyWords;
    for (const auto& message : messages)
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
                                   "VALUES({}, '{}', '{}');",
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
std::shared_ptr<InfoCollection>
DataBase::SELECT(const char query_str[]) const
{
    auto result = std::make_shared<InfoCollection>();
    QSqlQuery query(this->db);
    query.exec(query_str);
    quint8 columns_count = query.record().count();
    result->reserve(query.size());
    while (query.next())
    {
        InfoContainer new_row;
        for (quint8 i = 0; i < columns_count; i++)
            new_row[query.record().field(i).name()] = query.value(i);
        result->push_back(new_row);
    }
    return result;
}

// private
std::shared_ptr<InfoContainer>
DataBase::singleSELECT(const char query_str[]) const
{
    auto result = std::make_shared<InfoContainer>();
    QSqlQuery query(this->db);
    query.exec(query_str);
    if (query.next())
        for (quint8 i = 0; i < query.record().count(); i++)
            result->insert(query.record().field(i).name(), query.value(i).toString());
    return result;
}

// private
bool DataBase::execOtherQry(const char query_str[])
{
    QSqlQuery query(this->db);
    return query.exec(query_str);
}


std::shared_ptr<QJsonArray>
DataBase::convertToNormalForm(const QJsonArray &data)
{
    auto target = std::make_shared<QJsonArray> ();
    auto fields = data[0].toArray();
    for (auto itter = data.constBegin() + 1; itter < data.constEnd(); itter++)
    {
        QJsonObject new_row;
        const QJsonArray& data_row = itter->toArray();
        for (uint8_t i = 0; i < fields.size(); i++)
            new_row[fields[i].toString()] = data_row[i];
        target->append(new_row);
    }
    return target;
};

// private
bool DataBase::envExists(const quint64 &env_id) const
{
    return this->SELECT(fmt::format("SELECT * FROM chat_envs WHERE env_id={}",
                                    (uint64_t)env_id).c_str()
                        )->isEmpty();
}

bool DataBase::isValidPrivateChat(const quint64 &env_id) const
{
    auto result = this->singleSELECT(fmt::format("SELECT * FROM private_chats "
                                                 "WHERE env_id={};", env_id).c_str());
    return !(result->isEmpty());
}

bool DataBase::isPendingPrivateChat(const quint64 &env_id) const
{
    auto result = this->singleSELECT(fmt::format("SELECT * FROM pending_chat_envs "
                                                 "WHERE env_id={};", env_id).c_str());
    return !(result->isEmpty());
}

QString DataBase::getOtherAudienceNameInPrivateChat(const quint64 &private_env_id,
                                                    const bool& is_pending) const
{
    using namespace KeyWords;
    std::shared_ptr<InfoContainer> user_info;
    if(is_pending)
        user_info = this->getPendingPrivateChatInfoByEnvId(private_env_id);
    else
        user_info = this->getValidPrivateChatInfoByEnvId(private_env_id);

    return user_info->value(FIRST_PERSON).toUInt() == this->m_user_info[USER_ID].toUInt() ?
                this->getNameOfUser(user_info->value(SECOND_PERSON).toUInt()) :
                this->getNameOfUser(user_info->value(FIRST_PERSON).toUInt());
}

// private
quint64 DataBase::getLastInsertId() const
{
    return this->singleSELECT("SELECT last_insert_rowid() as id;")->value("id").toUInt();
}

// private
quint64 DataBase::getMaxMessagesId() const
{
    return this->singleSELECT("SELECT max(message_id) "
                              "FROM messages as id;"
                              )->value("id").toUInt();
}

// private
void DataBase::convertToHash(InfoContainer &target,
                             const QJsonObject &source)
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}
