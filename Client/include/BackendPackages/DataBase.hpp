#pragma once
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QSqlRecord>
#include <QSqlField>
#include <QDir>
#include "DataHandler.hpp"

static char database_filename[] = "db.sqlite";
static char data_path[] = "/home/pedram/Desktop/PMessenger/Client/data/";

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
        "    env_id              INTEGER PRIMARY KEY"
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


class DataBase
{
    friend class DataHandler;

    QSqlDatabase db;
public:
    DataBase()
        : db(QSqlDatabase::addDatabase("QSQLITE"))
    {
        QDir dir;
        if(!dir.exists(data_path))
        {
            if (dir.mkpath(data_path))
                qCritical() << "Couldn't create folder." << data_path;
        }

        this->db.setDatabaseName(QString(data_path) + database_filename);
        if (!this->db.open())
            qCritical() << "Couldn't open database";
        if (!this->createTables())
            qCritical() << "Couldn't create tables";
    }

    bool createTables()
    {
        QSqlQuery create_table(this->db);
        return
        create_table.exec(users_table_creation) &&
        create_table.exec(chat_envs_table_creation) &&
        create_table.exec(private_chats_table_creation) &&
        create_table.exec(messages_table_creation);
    }
public:
    bool SELECT(QJsonArray& result_set, char query_str[]) const
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

    bool execOtherQry(const char query_str[])
    {
        QSqlQuery query(this->db);
        return query.exec(query_str);
    }

};




