#pragma once
#include <QObject>
#include <thread>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include "NetworkPackages/NetReceiver.hpp"
#include "NetworkPackages/NetSender.hpp"
#include "NetworkPackages/NetworkHandler.hpp"

#include "BackendPackages/DataHandler.hpp"


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

public slots:
    void sendRegisterReq(const QString& username, const QString& password);

    void sendLoginReq(const QString& username, const QString& password);


private slots:
    void handleNetMessage(QJsonObject msg);


signals:
    void enterResponseCaugth(bool outcome, QString note);

};
