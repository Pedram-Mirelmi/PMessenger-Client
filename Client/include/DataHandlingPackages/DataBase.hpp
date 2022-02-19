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



    bool validPrivateChatAlreadyExists(const quint64& user_id,
                                       InfoContainer& chat_info) const;

    bool pendingPrivateChatAlreadyExists(const quint64& user_id,
                                         InfoContainer& chat_info) const;

    void getPendingPrivateChatInfoByOtherUser(const quint64& user_id,
                                              InfoContainer& chat_info) const;

    void getPendingPrivateChatInfoByEnvId(const quint64& invalid_env_id,
                                          InfoContainer& chat_info) const;


    void getValidPrivateChatInfoByOtherUser(const quint64& user_id,
                                            InfoContainer& chat_info) const;

    void getValidPrivateChatInfoByEnvId(const quint64& env_id,
                                        InfoContainer& chat_info) const;

    void getEnvTextMessages(const quint64& env_id,
                            QVector<InfoContainer>& messages,
                            const bool& pending_env) const;

    void getAllRegisteredEnvs(QVector<InfoContainer>& envs);

    void getAllPendingEnvs(QVector<InfoContainer>& envs);

    QString getNameOfUser(const quint64& user_id) const;

    quint64 getLastEnvMessageId(const quint64 env_id) const;

private:
    static void convertToHash(InfoContainer &target,
                              const QJsonObject &source);

    void insertValidTextMessage(const QJsonObject& msg_info);

    bool tryToCreateDBFile();

    bool createTables();

    bool SELECT(QVector<InfoContainer>& result_set,
                const char query_str[]) const;

    bool singleSELECT(InfoContainer& result,
                      const char query_str[]) const;

    bool execOtherQry(const char query_str[]);

    QJsonArray convertToNormalForm(const QJsonArray& data) const;

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




