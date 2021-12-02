#pragma once
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <string>
#include <cmath>
#include <QObject>
#include "../NetworkPackages/NetReceiver.hpp"
#include "../NetworkPackages/NetSender.hpp"
//#include "../MainApp.hpp"
class MainApp;

class NetworkHandler : public QObject
{
    Q_OBJECT
    friend class MainApp;
    QTcpSocket* m_socket;
    const QHostAddress m_address;
    quint16 m_port;
    NetMessageReceiver* m_receiver;
public:
    NetMessageSender* m_sender;

    explicit NetworkHandler(QObject* parent, const QString& address = "127.0.0.1", quint16 port = 54000);

    void setAutoConnect(bool enable);

public slots:
    void connectToServer();
    void handleNewNetMessage(const QJsonObject& net_msg);

signals:
    void newDataArrived(const QJsonObject& msg);
    void newMessageArrived(const QJsonObject& msg);
    void entryNetMessageArrived(const QJsonObject& msg);
    void userSearchResultArrived(const QJsonObject& msg);
    void newChatCreationMsgArrived(const QJsonObject& msg);
    void messageConfirmationArrived(const QJsonObject& msg);
};
