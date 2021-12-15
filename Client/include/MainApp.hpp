#pragma once
#include <QObject>
#include <thread>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include "./NetworkPackages/NetworkHandler.hpp"
#include "./DataHandlingPackages/DataHandler.hpp"

static QJsonObject user_info;

class MainApp : public QObject
{
    Q_OBJECT
public:
    NetworkHandler* m_network_handler;
    DataHandler* m_data_handler;

private:
    QJsonObject m_user_info;
public:
    explicit MainApp (QObject* parent = nullptr, const QString& address = "127.0.0.1", const quint16 port = 54000);;


private slots:
    void initiateDb(const QJsonObject& net_msg);
};
