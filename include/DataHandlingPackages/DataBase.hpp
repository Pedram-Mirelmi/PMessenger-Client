#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlRecord>
#include <QSqlField>
#include <QDir>
#include <QFile>
#include <QObject>
#include "../Others/stringTools.hpp"
#include "../Others/TypeDefs.hpp"
#include <QSqlError>

struct Message;

class DataHandler;
class MainApp;

class DataBase : public QObject
{
    Q_OBJECT
    QSqlDatabase db;
    InfoContainer& m_user_info;
    friend class DataHandler;
    friend class MainApp;

public:
    DataBase(QObject* parent, InfoContainer& user_info);


    Q_INVOKABLE void
    tryToInsertUser(const NetInfoContainer& user_info);

    bool
    tryToInit();


    bool
    tryToCreateDBFile();


    bool
    createTables();


    void
    checkForAnyPrivateEnvUpdate(const NetInfoCollection& envs);


    void
    insertValidTextMessagesList(const QJsonArray& messages);


    void
    deletePendingChat(const quint64& invalid_id);

    void
    deletePendingTextMessage(const quint64& invalid_id);

    void
    insertValidPrivateEnv(const InfoContainer& env_info,
                          bool participates = true);


    quint64
    insertPendingPrivateChat(const quint64& user_id);


    quint64
    insertPendingTextMessage(const quint16& env_id,
                             const QString& message_text,
                             const bool& to_pending_env);

    void
    insertValidTextMessage(const InfoContainer& msg_info);


    bool
    setValidPrivateChatInfoIfExists(const quint64& user_id,
                                    InfoContainer& user_info) const;


    bool
    setInvalidPrivateChatInfoIfExists(const quint64& user_id,
                                      InfoContainer& user_info) const;


    InfoContainerPtr
    getPendingPrivateChatInfoByOtherUser(const quint64& user_id) const;


    InfoContainerPtr
    getPendingPrivateChatInfoByEnvId(const quint64& invalid_env_id) const;


    InfoContainerPtr
    getValidPrivateChatInfoByOtherUser(const quint64& user_id) const;


    InfoContainerPtr
    getValidPrivateChatInfoByEnvId(const quint64& env_id) const;


    InfoCollectionPtr
    getEnvTextMessages(const quint64& env_id,
                       const bool& is_pending) const;


    InfoCollectionPtr
    getAllValidEnvs();


    InfoCollectionPtr
    getAllPendingEnvs();


    QString
    getNameOfUser(const quint64& user_id) const;


    QString
    getOtherPersonNameInPrivateChat(const quint64& private_env_id,
                                      const bool& is_pending) const;


    quint64
    getLastEnvMessageId(const quint64 env_id) const;


    quint64
    getLastInsertId() const;


    quint64
    getMaxMessagesId() const;

    bool
    envExists(const quint64& env_id) const;


    bool
    isValidPrivateChat(const quint64& env_id) const;


    bool
    isPendingPrivateChat(const quint64& env_id) const;

private:

    InfoCollectionPtr
    SELECT(const char query_str[]) const;


    InfoContainerPtr
    singleSELECT(const char query_str[]) const;


    bool
    execOtherQry(const QString& query_str);

signals:
    void
    newValidTextMessageInserted(const InfoContainer& msg_info);

//    void
//    newValidPrivateEnvInserted(const quint64& invalid_env_id,
//                               const quint64& valid_env_id);

    void
    needPrivateEnvDetails(const quint64 &env_id); // faced new raw env_id, need info and messages

    void
    needUserInfo(const quint64& user_id) const;

    void
    needPrivateEnvMessages(quint64 env_id, quint64 last_message_we_have);
};




