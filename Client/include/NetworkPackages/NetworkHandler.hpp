#pragma once
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <string>
#include <cmath>
#include <QObject>
#include <QTimer>
#include "../NetworkPackages/NetReceiver.hpp"
#include "../NetworkPackages/NetSender.hpp"

class MainApp;

class NetworkHandler : public QObject
{
    Q_OBJECT
    friend class MainApp;

    QTcpSocket* m_socket;
    const QHostAddress m_address;
    quint16 m_port;
    NetMessageReceiver* m_receiver;
    NetMessageSender* m_sender;
    QThread* m_network_thread;
public:
    Q_PROPERTY(bool netConnected READ netConnected NOTIFY netConnectedChanged);
signals:
    void netConnectedChanged(bool connection_status);
public:
    explicit NetworkHandler(QObject* parent,
                            const QString& address = "127.0.0.1",
                            quint16 port = 54000);

    bool netConnected ();

    Q_INVOKABLE void sendRegisterReq(const QString& username,
                                     const QString& password);

    Q_INVOKABLE void sendLoginReq(const QString& username,
                                  const QString& password);

    Q_INVOKABLE void sendUsernameSearchReq(const QString& username);

    Q_INVOKABLE void test();

//    Q_INVOKABLE QString getLastMessageText(const quint64& env_id,
//                                           const bool& is_pending);

    void sendFetchReq();

    void sendCreateNewPrivateChatReq(const quint64& user_chat_with,
                                     const quint64& invalid_id);

    void sendNewTextMessageReq(const quint64& env_id,
                               const QString& message_text,
                               const quint64& invalid_id);

    void sendEnvDetailsReq(const quint64& env_id);

public slots:
    void connectToServer();
    void handleNewNetMessage(const QJsonObject& net_msg);

signals:
    void entrySuccessful(const QJsonObject& msg);
    void newDataArrived(const QJsonObject& msg);
    void entryNetMessageArrived(const QJsonObject& msg);
    void newChatCreationMsgArrived(const QJsonObject& msg);
};
