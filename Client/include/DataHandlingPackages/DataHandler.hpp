#pragma once
#include <memory>
#include <string>
#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "../../Commons/KeyWords.hpp"
#include "DataBase.hpp"
#include "../NetworkPackages/NetworkHandler.hpp"
#include "./models/ConversationsListModel.hpp"
#include "./models/MessageListModel.hpp"

class MainApp;

class DataHandler : public QObject
{
    Q_OBJECT
    friend class MainApp;

    DataBase* m_db;
    NetworkHandler* m_net_handler;
    MessageListModel* m_message_list_model;
    ConversationsListModel* m_conversation_list_model;

public:
    explicit DataHandler(QObject* parent, NetworkHandler* netHandler);

    void feedNewMessagesToModel(const int& env_id);


public slots:
    void handleNewData(const QJsonObject& net_message);


private:
    void handleFetchResult(const QJsonObject& net_message);

    void sendReqForPrivateEnvDetails(const int& env_id);

    void startDB();

signals:
    void newMessageReceived(const QJsonObject& message_info);

private:
    void convertToHash(InfoContainer& target, const QJsonObject& source);
};











