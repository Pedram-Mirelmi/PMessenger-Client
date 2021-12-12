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


class MainApp;

class DataHandler : public QObject
{
    Q_OBJECT
//    friend class MainApp;

    DataBase* m_db;
    NetworkHandler* m_net_handler;
public:
    explicit DataHandler(QObject* parent, NetworkHandler* netHandler);

    void startDB();

public slots:
    void handleNewData(const QJsonObject& net_message);


private slots:
    void addNewMessageToModel(const QJsonObject& msg_info);

    void addNewChatEnvToModel(const QJsonObject& env_info);



private:
    void handleFetchAllResult(const QJsonObject& net_message);

    void sendReqForChatEnvMessages(const int& env_id);


signals:
    void newMessageReceived(const QJsonObject& message_info);

private:

};











