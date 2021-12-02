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
#include "../Others/format.hpp"

class MainApp;

class DataHandler : public QObject
{
    Q_OBJECT
    friend class MainApp;

    DataBase m_db;
    NetworkHandler* m_net_handler;
public:
    DataHandler(QObject* parent, NetworkHandler* netHandler);

    void startDB();

public slots:
    void handleNewData(const QJsonObject& net_message);

private:
    void handleFetchAllResult(const QJsonObject& net_message);

    void addNewChatEnv(const QJsonObject& net_message);

    void sendReqForChatEnvDetails(const int& env_id);

    void sendFetchAllReq();

    void insertNewMessage(const QJsonObject& message);
signals:
    void newMessageReceived(const QJsonObject& message_info);
};











