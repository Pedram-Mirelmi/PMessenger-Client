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


    Q_INVOKABLE void tryToInsertUser(const quint64 &user_id,
                                     const QString &username,
                                     const QString &name);

    bool tryToInit();

    void tryToInsertPrivateEnvs(const QJsonValueRef& envs);


    void deletePendingChat(const quint64& invalid_id);

    void insertPrivateEnvs(const QJsonValueRef& envs);

    void insertSinglePrivateEnv(const QJsonObject& env_info);

    void insertValidTextMessages(const QJsonArray& messages);

    quint16 insertNewPendingPrivateChat(const quint64& user_id);

    quint16 insertPendingTextMessage(const quint16& env_id,
                                     const QString& message_text,
                                     const bool& to_pending_env);



    bool setValidPrivateChatInfoIfExists(const quint64& user_id,
                                    InfoContainer& user_info) const;

    bool setInvalidPrivateChatInfoIfExists(const quint64& user_id,
                                           InfoContainer& user_info) const;

    std::shared_ptr<InfoContainer>
    getPendingPrivateChatInfoByOtherUser(const quint64& user_id) const;

    std::shared_ptr<InfoContainer>
    getPendingPrivateChatInfoByEnvId(const quint64& invalid_env_id) const;

    std::shared_ptr<InfoContainer>
    getValidPrivateChatInfoByOtherUser(const quint64& user_id) const;

    std::shared_ptr<InfoContainer>
    getValidPrivateChatInfoByEnvId(const quint64& env_id) const;

    std::shared_ptr<InfoCollection>
    getEnvTextMessages(const quint64& env_id,
                       const bool& pending_env) const;

    std::shared_ptr<InfoCollection>
    getAllRegisteredEnvs();

    std::shared_ptr<InfoCollection>
    getAllPendingEnvs();

    QString getNameOfUser(const quint64& user_id) const;

    quint64 getLastEnvMessageId(const quint64 env_id) const;

private:
    static void convertToHash(InfoContainer &target,
                              const QJsonObject &source);

    void insertValidTextMessage(const QJsonObject& msg_info);

    bool tryToCreateDBFile();

    bool createTables();

    std::shared_ptr<InfoCollection>
    SELECT(const char query_str[]) const;

    std::shared_ptr<InfoContainer>
    singleSELECT(const char query_str[]) const;

    bool execOtherQry(const char query_str[]);

    static std::shared_ptr<QJsonArray>
    convertToNormalForm(const QJsonArray& data);

    bool envExists(const quint64& env_id) const;

    bool isValidPrivateChat(const quint64& env_id) const;

    bool isPendingPrivateChat(const quint64& env_id) const;

    QString getOtherAudienceNameInPrivateChat(const quint64& private_env_id,
                                              const bool& is_pending) const;

    quint64 getLastInsertId() const;

    quint64 getMaxMessagesId() const;

signals:
    void newTextMessageInserted(const QJsonObject& msg_info);

    void needPrivateEnvDetails(const quint64 &env_id); // need info and messages

};




