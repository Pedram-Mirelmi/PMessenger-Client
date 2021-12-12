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

class DataHandler;

class DataBase : public QObject
{
    Q_OBJECT

    QSqlDatabase db;

    friend class DataHandler;
public:
    DataBase(QObject* parent);

    bool tryToInit();

    bool insertPrivateEnv(const QJsonObject& env);

    bool insertGroupEnv(const QJsonObject& env);

    bool insertChannelEnv(const QJsonObject& env);

    void insertPrivateMessages(const QJsonArray& messages);

    void insertGroupMessages(const QJsonArray& messages);

    void insertChannelMessages(const QJsonArray& messages);

private:

    void insertSinglePrivateMessage(const QJsonObject& msg_info);

    bool tryToCreateDBFile();

    bool createTables();

    bool SELECT(QJsonArray& result_set, char query_str[]) const;

    bool execOtherQry(const char query_str[]);

    QJsonArray convertToNormalForm(const QJsonArray& data);

signals:
    void newMessageInserted(const QJsonObject& msg_info);
};




