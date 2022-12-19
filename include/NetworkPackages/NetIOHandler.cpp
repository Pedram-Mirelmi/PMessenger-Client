#include "./NetIOHandler.hpp"
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QThread>
#include <future>
#include <string>
#include <KeyWords.hpp>
#include "../Others/CommonTools.hpp"

// constructor
NetIOHandler::NetIOHandler(QObject *parent,
                               const std::string &address,
                               quint16 port)
    : QObject(parent),
      AbstractNetIOManager<NetMessageType>(address, port)

{

}


// public
bool NetIOHandler::netConnected()
{
    return m_isConnected;
}

void NetIOHandler::requestRegister(const QString &username, const QString &password)
{

}

void NetIOHandler::requestLogin(const QString &username, const QString &password)
{

}

void NetIOHandler::requestUsernameSearch(const QString &username)
{

}

void NetIOHandler::requestFetch()
{

}

void NetIOHandler::requestNewPrivateChat(const quint64 &user_chat_with, const quint64 &invalid_id)
{

}

void NetIOHandler::requestNewTextMessage(const quint64 &env_id, const QString &message_text, const quint64 &invalid_id)
{

}

void NetIOHandler::requestEnvDetails(const quint64 &env_id)
{

}

void NetIOHandler::requestEnvMessages(const quint64 &env_id, const quint64 &last_message_we_have)
{

}


// public slot
void NetIOHandler::handleNewNetMessage(const QJsonObject &net_msg)
{

}



void NetIOHandler::sendUserInfoReq(const quint64 &user_id)
{
    using namespace KeyWords;

}

void NetIOHandler::start()
{

}

void NetIOHandler::stop()
{

}

void NetIOHandler::onNewMessageReadCompletely()
{

}
