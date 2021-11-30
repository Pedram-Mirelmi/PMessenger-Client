#pragma once
#include <memory>
#include <fmt/format.h>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../Commons/KeyWords.h"
#include "DataBase.hpp"
#include "../NetworkPackages/NetworkHandler.hpp"
#include "../MainApp.hpp"

class DataHandler : public QObject
{
    Q_OBJECT
    friend class MainApp;

    DataBase m_db;
    NetworkHandler* m_net_handler;
public:
    DataHandler(QObject* parent, NetworkHandler* netHandler)
        :QObject(parent), m_net_handler(netHandler)
    {
        QObject::connect(m_net_handler, &NetworkHandler::newDataArrived, this, &DataHandler::handleNewData);
    }


public slots:
    void handleNewData(const QJsonObject& net_message)
    {
        using namespace KeyWords;
        auto data_type = net_message[DATA_DETAILS];
        if (data_type == FETCH_ALL_RESULT)
        {
            this->handleFetchAllResult(net_message);
        }
        else if (data_type == FETCH_RESULT)
        {

        }
        else if (data_type == CHAT_ENV_DETAILS_RESULT)
        {
            this->addNewChatEnv(net_message);
        }
        else if (data_type == MESSAGE)
        {
            for (const auto& message_info : net_message[DATA].toArray())
                insertNewMessage(message_info.toObject());
        }
    }

private:
    void handleFetchAllResult(const QJsonObject& net_message)
    {
        using namespace KeyWords;
        auto chat_envs_list = net_message[ALL_CHAT_ENV_IDS].toArray();
        for (const auto& env_id : chat_envs_list)
        {
            auto query = fmt::format("INSERT INTO chat_envs(env_id, participates) VALUES({}, 1);", env_id.toInt());
            bool ok = this->m_db.execOtherQry(query.c_str());
            if (ok)
                this->sendReqForChatEnvDetails(env_id.toInt());
        }
    }

    void addNewChatEnv(const QJsonObject& net_message)
    {
        using namespace KeyWords;
        if (net_message[CHAT_ENV_TYPE] == PRIVATE_CHAT)
        {
            auto query = fmt::format("INSERT INTO private_chats(env_id, first_person, second_person) VALUES({}, {}, {});",
                                        net_message[ENV_ID].toInt(), net_message[FIRST_PERSON].toInt(), net_message[SECOND_PERSON].toInt());
            this->m_db.execOtherQry(query.c_str());
        }
    }

    void sendReqForChatEnvDetails(const int& env_id)
    {
        using namespace KeyWords;
        QJsonObject req;
        req[MESSAGE_TYPE] = GET_CHAT_ENV_DETAILS;
        req [ENV_ID] = env_id;
        this->m_net_handler->m_sender->sendMessage(req);
    }

    void insertNewMessage(const QJsonObject& message)
    {
        using namespace KeyWords;
        bool ok;
        if (message.contains(CREATED_AT))
        {
            std::string query = fmt::format(
                    "INSERT INTO messages(message_id, owner_id, env_id, message_text, created_at) VALUES "
                    "                     ({},        {},       {},     '{}',         '{}' )",
                    message[MESSAGE_ID].toInt(), message[OWNER_ID].toInt(),message[ENV_ID].toInt(),
                    message[MESSAGE_TEXT].toString(), message[CREATED_AT].toString());
            ok = this->m_db.execOtherQry(query.c_str());
        }
        else
        {
            std::string query = fmt::format(
                    "INSERT INTO messages(message_id, owner_id, env_id, message_text) VALUES "
                    "                     ({},        {},       {},     '{}',         '{}' )",
                    message[MESSAGE_ID].toInt(), message[OWNER_ID].toInt(),message[ENV_ID].toInt(), message[MESSAGE_TEXT].toString());
            ok = this->m_db.execOtherQry(query.c_str());
        }
        if (!message[SEEN].toBool() && ok)
        {
            emit newMessageReceived(message);
        }
    }
signals:
    void newMessageReceived(const QJsonObject& message_info);
};











