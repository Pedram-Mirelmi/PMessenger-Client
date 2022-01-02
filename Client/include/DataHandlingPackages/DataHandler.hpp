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
    quint64 this_user_id;

    DataBase* m_db;
    NetworkHandler* m_net_handler;
    MessageListModel* m_message_list_model;
    ConversationsListModel* m_conversation_list_model;

public:
    explicit DataHandler(QObject* parent, NetworkHandler* netHandler);

    void feedNewMessagesToModel(const int& env_id);

    Q_INVOKABLE void startNewPrivateChat(const quint64 &user_id);

    Q_INVOKABLE void sendNewTextMessage(const quint64& env_id, const QString& message_text);


public slots:
    void handleNewData(const QJsonObject& net_message);


private:
    void handleFetchResult(const QJsonObject& net_message);

    void sendReqForPrivateEnvDetails(const quint64& env_id);

    void prepareDB();

// function for pending fils
    bool tryToCreatePendingFiles();

    void readPendingsFromFile();

    void addPrivateChatToPendingChats(const quint64& user_id);

    void addNewTextMessageToPendingMessages(const quint64& env_id, const QString& message_text);

    template<typename Loader_T>
    bool deserializeFromFile(Loader_T& data_to_deserialize, const char* file_path);

    template<typename Loader_T>
    bool serializeToFile(Loader_T& data_to_serialize, const char * file_path);

private:
    void convertToHash(InfoContainer& target, const QJsonObject& source);
};











