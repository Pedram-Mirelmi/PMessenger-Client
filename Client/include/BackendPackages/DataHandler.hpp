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
#include "../Commons/KeyWords.h"
#include "DataBase.hpp"
#include "../NetworkPackages/NetworkHandler.hpp"


class MainApp;

class DataHandler : public QObject
{
    Q_OBJECT
//    friend class MainApp;

    DataBase m_db;
    NetworkHandler* m_net_handler;
public:
    DataHandler(QObject* parent, NetworkHandler* netHandler);

    void startDB();

public slots:
    void handleNewData(const QJsonObject& net_message);

private:
    void handleFetchAllResult(const QJsonObject& net_message);

    void sendReqForChatEnvMessages(const int& env_id);

    bool insertPrivateEnv(const QJsonObject& env);

    bool insertGroupEnv(const QJsonObject& env);

    bool insertChannelEnv(const QJsonObject& env);

    void insertPrivateMessages(const QJsonArray& messages);

    void insertGroupMessages(const QJsonArray& messages);

    void insertChannelMessages(const QJsonArray& messages);
signals:
    void newMessageReceived(const QJsonObject& message_info);

private:
    QJsonArray convertToNormalForm(const QJsonArray& data);
};











