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
#include "../../KeyWords.hpp"
#include "DataBase.hpp"
#include "../NetworkPackages/NetworkHandler.hpp"
#include "./models/ConversationsListModel.hpp"
#include "./models/MessageListModel.hpp"

class MainApp;

class DataHandler : public QObject
{
    Q_OBJECT
    friend class MainApp;
    InfoContainer& m_user_info;

    DataBase* m_db;
    NetworkHandler* m_net_handler;
    MessageListModel* m_message_list_model;
    ConversationsListModel* m_conversation_list_model;


public:
    explicit DataHandler(QObject* parent,
                         NetworkHandler* netHandler,
                         InfoContainer& user_info);


    void
    prepareDB();



public slots:
    void
    handleNewData(const QJsonObject& net_message);


    void
    registerAllPendingChats();


    void
    registerAllMessages();


public:


    void
    handleFetchResult(const QJsonObject& net_message);


    quint64
    createNewPrivateChat(const quint64& user_id,
                         const QString& name);

    void
    validatePrivateChat(const NetInfoContainer& env_info);


    void
    feedEnvMessagesToMessagesModel(const quint64& env_id,
                                   const bool& is_pending_env);


    void
    fillConversationListModel();



    QString
    getProperConversationHeader(const quint64& env_id,
                                const bool& is_pending);


signals:

    void
    searchUsernameResultArrived(const QJsonArray& search_result);

};


