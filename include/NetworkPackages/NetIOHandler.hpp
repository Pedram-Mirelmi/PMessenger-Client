#pragma once
#include <QTcpSocket>
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <string>
#include <cmath>
#include <QObject>
#include <QTimer>
#include "../AsyncNetFramework/AbstractNetIOManager.h"
#include "../NetworkPackages/NetMessages/NetMessageType.hpp"
#include "../Others/TypeDefs.hpp"


class MainApp;

class NetIOHandler : public QObject,
                       public AbstractNetIOManager<NetMessageType>
{
    Q_OBJECT
public:
    Q_PROPERTY(bool netConnected READ netConnected NOTIFY netConnectedChanged);
signals:
    void netConnectedChanged(bool connection_status);
public:
    explicit NetIOHandler(QObject* parent,
                            const std::string &address = "127.0.0.1",
                            quint16 port = 54000);

    bool netConnected ();

    void requestRegister(const QString& username,
                                     const QString& password);

    void requestLogin(const QString& username,
                                  const QString& password);

    void requestUsernameSearch(const QString& username);

//    Q_INVOKABLE QString getLastMessageText(const quint64& env_id,
//                                           const bool& is_pending);

    void requestFetch();

    void requestNewPrivateChat(const quint64& user_chat_with,
                                     const quint64& invalid_id);

    void requestNewTextMessage(const quint64& env_id,
                               const QString& message_text,
                               const quint64& invalid_id);

    void requestEnvDetails(const quint64& env_id);

    void requestEnvMessages(const quint64& env_id, const quint64& last_message_we_have);

public slots:
    void handleNewNetMessage(const QJsonObject& net_msg);
    void sendUserInfoReq(const quint64& user_id);

signals:
    void entrySuccessful(const QJsonObject& msg);
    void newDataArrived(const QJsonObject& msg);
    void entryNetMessageArrived(const QJsonObject& msg);
    void newChatCreationMsgArrived(const QJsonObject& msg);
    void usernameSearchResultArrived(const QJsonArray& results);
    void serverConfirmedPrivateChatCreation(const InfoContainer& env_info);
    void serverConfirmedTextMessageCreation(const InfoContainer& message_info);

    // IService interface
public:
    void start() override;
    void stop() override;

    // AbstractNetIOManager interface
protected:
    void onNewMessageReadCompletely() override;
};
