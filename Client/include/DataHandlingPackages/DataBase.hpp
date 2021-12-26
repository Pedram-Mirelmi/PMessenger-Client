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

struct Message;

class DataHandler;

class DataBase : public QObject
{
    Q_OBJECT

    QSqlDatabase db;

    friend class DataHandler;
public:
    DataBase(QObject* parent);

    bool tryToInit();

    void tryToInsertPrivateEnvs(const QJsonValueRef& envs);

    void insertPrivateEnvs(const QJsonValueRef& envs);

    void insertSinglePrivateEnv(const QJsonObject& env_info);

    void insertTextMessages(const QJsonArray& messages);

    void insertGroupMessages(const QJsonArray& messages);

    bool insertGroupEnv(const QJsonObject& env);

    bool insertChannelEnv(const QJsonObject& env);

    void insertChannelMessages(const QJsonArray& messages);

private:
    void convertToHash(InfoContainer &target, const QJsonObject &source);

    void insertSingleTextMessage(const QJsonObject& msg_info);

    bool tryToCreateDBFile();

    bool createTables();

    bool SELECT(QVector<InfoContainer>& result_set, const char query_str[]) const;

    bool execOtherQry(const char query_str[]);

    QJsonArray convertToNormalForm(const QJsonArray& data);

    bool envExists(const quint64& env_id) const;

signals:
    void newTextMessageInserted(const QJsonObject& msg_info);

    void needPrivateEnvDetails(const quint64 &env_id); // need info and messages
};




