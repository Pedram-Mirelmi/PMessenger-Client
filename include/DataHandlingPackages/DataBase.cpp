#include "DataBase.hpp"
#include "../../KeyWords.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <memory>

static char data_path[] = "../data/";
static char database_filename[] = "db.sqlite";
static char sql_codes_filename[] = "SqliteQueries.sql";

QMutex insert_lock; // to get last_insert_rowid() thead safly


DataBase::DataBase(QObject* parent, InfoContainer& user_info)
    :QObject(parent),
      db(QSqlDatabase::addDatabase("QSQLITE")),
      m_user_info(user_info)
{}


// Q_INVOKABLE
void
DataBase::tryToInsertUser(const NetInfoContainer& user_info)
{
    using namespace KeyWords;
    this->execOtherQry(fmt::format("INSERT INTO users(user_id, username, name) "
                                   "VALUES({}, '{}', '{}');",
                                   user_info[USER_ID].toInteger(),
                                   user_info[USERNAME].toString().toStdString(),
                                   user_info[NAME].toString().toStdString().c_str()).c_str());
}


bool
DataBase::tryToInit()
{
    using namespace KeyWords;
    QFile file;
    qDebug() << QDir::currentPath();
    qDebug() << QString(data_path) + this->m_user_info[USERNAME].toString() + ".sqlite";
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


bool
DataBase::tryToCreateDBFile()
{
    QFile db_file(QString(data_path) + database_filename);
    return db_file.open(QFile::ReadWrite);
}


bool
DataBase::createTables()
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


void
DataBase::checkForAnyPrivateEnvUpdate(const NetInfoCollection &envs)
{
    using namespace KeyWords;
    for (auto itter = envs.constBegin(); itter != envs.constEnd(); itter++)
    {
        auto env_little_info = itter->toArray(); // the info : [env_id, last_message_id]
        if (!envExists(env_little_info[0].toInteger()))
        {
            emit this->needPrivateEnvDetails(itter->toInteger());
        }
        else if(!env_little_info[1].isNull() &&
                getLastEnvMessageId(env_little_info[0].toInteger()) != (quint64)env_little_info[1].toInteger())
        {
            emit this->needPrivateEnvMessages(env_little_info[0].toInteger(), getLastEnvMessageId(env_little_info[0].toInteger()));
        }

    }
}

void
DataBase::insertValidTextMessagesList(const NetInfoCollection &messages)
{
    using namespace KeyWords;
    QMutexLocker scoped_lock(&insert_lock);
    auto messages_list = DataBase::convertToNormalForm(messages);
    for (auto itter = messages_list->begin(); itter < messages_list->end(); itter++)
        this->insertValidTextMessage(itter->toObject());
}


void
DataBase::deletePendingChat(const quint64 &invalid_id)
{
    this->execOtherQry(fmt::format("DELETE FROM pending_chat_envs"
                                   " WHERE invalid_env_id={}", invalid_id).c_str());
}


void
DataBase::insertValidPrivateEnv(const NetInfoContainer &env_info, bool participates)
{
    using namespace KeyWords;
    auto other_person = env_info[OTHER_PERSON_INFO].toObject();
    this->tryToInsertUser(other_person);
    const auto create_env_query = fmt::format("INSERT INTO chat_envs(env_id, participates) "
                                              "VALUES({}, {});",
                                              env_info[ENV_ID].toInteger(), (int)participates);
    const auto create_private_query = fmt::format("INSERT INTO "
                                                  "private_chats(env_id, other_person) "
                                                  "VALUES       ({},       {});",
                                                  env_info[ENV_ID].toInteger(),
                                                  other_person[USER_ID].toInteger(),
                                                  env_info[SECOND_PERSON].toInteger());
    if (this->execOtherQry(create_env_query.c_str()) &&
            this->execOtherQry(create_private_query.c_str()))
    {
        emit this->newValidPrivateEnvInserted(env_info,
                                              other_person[NAME].toString(),
                                              this->getLastEnvMessageId(env_info[ENV_ID].toInteger()));
    }
}


quint16
DataBase::insertPendingPrivateChat(const quint64 &user_id)
{
    using namespace KeyWords;
    QMutexLocker scoped_lock(&insert_lock);
    this->execOtherQry(fmt::format("INSERT INTO pending_chat_envs(env_type, first_person, second_person) "
                                   "VALUES('private_chat', {}, {});",
                                   this->m_user_info[USER_ID].toUInt(), user_id).c_str()
                       );
    auto inserted_invalid_id = this->getLastInsertId();
    return inserted_invalid_id;
}


quint16
DataBase::insertPendingTextMessage(const quint16 &env_id,
                                   const QString &message_text,
                                   const bool& to_pending_env)
{
    QMutexLocker scoped_lock(&insert_lock);
    auto id_field = to_pending_env ? "invalid_env_id" : "env_id";
    this->execOtherQry(fmt::format("INSERT INTO pending_messages({}, message_text) "
                                   "VALUES({}, '{}');",
                                   id_field, env_id,
                                   toRaw(message_text.toStdString())).c_str()
                       );
    auto inserted_id = getLastInsertId();
    return inserted_id;
}



void
DataBase::insertValidTextMessage(const QJsonObject& msg_info)
{
    using namespace KeyWords;
    QMutexLocker scoped_lock(&insert_lock);
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
        emit this->newValidTextMessageInserted(msg_info);
}


bool
DataBase::setValidPrivateChatInfoIfExists(const quint64 &user_id,
                                          InfoContainer& user_info) const
{

    auto info = this->getValidPrivateChatInfoByOtherUser(user_id);
    if (info->isEmpty())
        return false;
    user_info = *info;
    return true;
}


bool
DataBase::setInvalidPrivateChatInfoIfExists(const quint64 &user_id,
                                            InfoContainer& user_info) const
{
    auto info = this->getPendingPrivateChatInfoByOtherUser(user_id);
    if(info->isEmpty())
        return false;
    user_info = *info;
    return true;
}


InfoContainerPtr
DataBase::getPendingPrivateChatInfoByOtherUser(const quint64 &user_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM pending_chat_envs "
                                          "WHERE first_person = {} OR second_person = {}",
                                          user_id, user_id).c_str());
}


InfoContainerPtr
DataBase::getPendingPrivateChatInfoByEnvId(const quint64 &invalid_env_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM pending_chat_envs "
                                          "WHERE invalid_env_id = {}",
                                          invalid_env_id).c_str());
}


InfoContainerPtr
DataBase::getValidPrivateChatInfoByOtherUser(const quint64 &user_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM private_chats "
                                          "WHERE first_person = {} OR second_person = {}",
                                          user_id, user_id).c_str());
}


InfoContainerPtr
DataBase::getValidPrivateChatInfoByEnvId(const quint64 &env_id) const
{
    return this->singleSELECT(fmt::format("SELECT * FROM private_chats "
                                          "WHERE env_id = {}",
                                          env_id).c_str());
}


InfoCollectionPtr
DataBase::getEnvTextMessages(const quint64 &env_id,
                             const bool& is_pending) const
{
    auto result = std::make_shared<InfoCollection>();
    if (is_pending)
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


InfoCollectionPtr
DataBase::getAllValidEnvs()
{
    return this->SELECT("SELECT * FROM private_chats");
}


InfoCollectionPtr
DataBase::getAllPendingEnvs()
{
    return this->SELECT("SELECT * FROM pending_chat_envs");
}


QString
DataBase::getNameOfUser(const quint64 &user_id) const
{
    auto user_info = this->singleSELECT(fmt::format("SELECT name FROM users "
                                                    "WHERE user_id = {}",
                                                    user_id).c_str()
                                        );
    if(user_info->isEmpty())
    {
        emit this->needUserInfo(user_id);
        return "Unknown";
    }

    return user_info->value(KeyWords::NAME).toString();
}


QString
DataBase::getOtherPersonNameInPrivateChat(const quint64 &private_env_id,
                                            const bool& is_pending) const
{
    using namespace KeyWords;
    InfoContainerPtr env_info;
    env_info = is_pending ? this->getPendingPrivateChatInfoByEnvId(private_env_id) :
                             this->getValidPrivateChatInfoByEnvId(private_env_id);

    return this->getNameOfUser((*env_info)[OTHER_PERSON_INFO].toJsonObject()[USER_ID].toInteger());
}

quint64
DataBase::getLastEnvMessageId(const quint64 env_id) const
{
    auto info = this->singleSELECT(fmt::format("SELECT MAX(message_id) AS id "
                                               "FROM messages "
                                               "WHERE env_id = {};", env_id).c_str());
    return info->contains("id") ? info->value("id").toUInt() : 0;
}



quint64
DataBase::getLastInsertId() const
{
    return this->singleSELECT("SELECT last_insert_rowid() as id;")->value("id").toUInt();
}

quint64
DataBase::getMaxMessagesId() const
{
    return this->singleSELECT("SELECT max(message_id) "
                              "FROM messages as id;"
                              )->value("id").toUInt();
}


bool
DataBase::envExists(const quint64 &env_id) const
{
    return !(this->SELECT(fmt::format("SELECT * FROM chat_envs WHERE env_id={}",
                                      (uint64_t)env_id).c_str())->isEmpty());
}


bool
DataBase::isValidPrivateChat(const quint64 &env_id) const
{
    return !(this->singleSELECT(fmt::format("SELECT * FROM private_chats "
                                            "WHERE env_id={};", env_id).c_str())
            )->isEmpty();
}


bool
DataBase::isPendingPrivateChat(const quint64 &env_id) const
{
    return !(this->singleSELECT(fmt::format("SELECT * FROM pending_chat_envs "
                                            "WHERE env_id={};", env_id).c_str())
            )->isEmpty();
}



void
DataBase::convertToHash(InfoContainer &target,
                        const QJsonObject &source)
{
    for (auto itter = source.constBegin(); itter < source.constEnd(); itter++)
        target[itter.key()] = itter.value().toVariant();
}


NetInfoCollectionPtr
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




InfoCollectionPtr
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


InfoContainerPtr
DataBase::singleSELECT(const char query_str[]) const
{
    auto result = std::make_shared<InfoContainer>();
    QSqlQuery query(this->db);
    query.exec(query_str);
    if (query.next())
        for (quint8 i = 0; i < query.record().count(); i++)
            result->insert(query.record().field(i).name(), query.value(i));
    return result;
}

bool
DataBase::execOtherQry(const QString& query_str)
{
    QSqlQuery query(this->db);
    return query.exec(query_str);
}

