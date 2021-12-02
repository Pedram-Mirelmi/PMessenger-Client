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


class DataHandler;

class DataBase
{
    friend class DataHandler;

    QSqlDatabase db;
public:
    DataBase();

    bool tryToInit();

private:
    bool tryToCreateDBFile();

    bool createTables();
public:
    bool SELECT(QJsonArray& result_set, char query_str[]) const;

    bool execOtherQry(const char query_str[]);

};




