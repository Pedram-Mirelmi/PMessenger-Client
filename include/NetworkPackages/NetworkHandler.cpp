#include "./NetworkHandler.hpp"
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QThread>
#include <future>

// constructor
NetworkHandler::NetworkHandler(QObject *parent,
                               const QString &address,
                               quint16 port)
    : QObject(parent),
      m_socket(new QTcpSocket(this)),
      m_address(address),
      m_port(port),
      m_receiver(new NetMessageReceiver(nullptr, this->m_socket)),
      m_sender(new NetMessageSender(nullptr, this->m_socket)),
      m_network_thread(new QThread(this))
{
    this->m_receiver->moveToThread(this->m_network_thread);
    this->m_sender->moveToThread(this->m_network_thread);
    this->m_network_thread->start();

    QObject::connect(this->m_socket, &QTcpSocket::disconnected,
                     this->m_receiver, &NetMessageReceiver::stopListening,
                     (Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection));

    QObject::connect(this->m_socket, &QTcpSocket::connected,
                     this->m_receiver, &NetMessageReceiver::startListening,
                     (Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection));

    QObject::connect(this->m_receiver, &NetMessageReceiver::newNetMessageArrived,
                     this, &NetworkHandler::handleNewNetMessage,
                     (Qt::ConnectionType)(Qt::QueuedConnection | Qt::UniqueConnection));

    QObject::connect(this->m_socket, &QTcpSocket::stateChanged,
                     [&](const QTcpSocket::SocketState& new_state)
                     {
                         static unsigned long tries_count = 0;
                         qDebug() << new_state;
                         if(new_state == QTcpSocket::SocketState::ConnectedState)
                             emit this->netConnectedChanged(true);
                         else if(new_state == QTcpSocket::SocketState::UnconnectedState)
                         {
                             emit this->netConnectedChanged(false);
                             qDebug() << "not connected... trying ..." << tries_count++;
                             QTimer::singleShot(500, this, &NetworkHandler::connectToServer);
                         }
                     }
    );

    this->connectToServer();
}


// public
bool NetworkHandler::netConnected()
{
    return this->m_socket->state() == QAbstractSocket::ConnectedState;
}


// Q_INVOKABLE
void NetworkHandler::sendRegisterReq(const QString &username, const QString &password)
{
    QJsonObject request;
    using namespace KeyWords;
    request[NET_MESSAGE_TYPE] = REGISTER;
    request[USERNAME] = username;
    request[PASSWORD] = password;
    this->m_sender->sendNetMessage(request);
}

// Q_INVOKABLE
void NetworkHandler::sendLoginReq(const QString &username, const QString &password)
{
    QJsonObject request;
    using namespace KeyWords;
    request[NET_MESSAGE_TYPE] = LOGIN;
    request[USERNAME] = username;
    request[PASSWORD] = password;
    this->m_sender->sendNetMessage(request);
}

void NetworkHandler::sendUsernameSearchReq(const QString &username)
{
    using namespace KeyWords;
    QJsonObject net_msg;
    net_msg[NET_MESSAGE_TYPE] = SEARCH_USERNAME;
    net_msg[USERNAME_TO_SEARCH] = username;
    this->m_sender->sendNetMessage(net_msg);
}

void NetworkHandler::test()
{
    qDebug() << this->m_socket->state();
    QThread::sleep(10);
}

// Q_INVOKSBLE
void NetworkHandler::sendFetchReq()
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = FETCH;
    this->m_sender->sendNetMessage(req);
}

void NetworkHandler::sendCreateNewPrivateChatReq(const quint64 &user_chat_with,
                                                 const quint64& invalid_env_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = CREATE_NEW_PRIVATE_CHAT;
    req[USER_ID] = (qint64)user_chat_with;
    req[INVALID_ENV_ID] = (qint64)invalid_env_id;
    this->m_sender->sendNetMessage(req);
}

void NetworkHandler::sendNewTextMessageReq(const quint64& env_id,
                                           const QString& message_text,
                                           const quint64& invalid_message_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = NEW_TEXT_MESSAGE;
    req[MESSAGE_TEXT] = message_text;
    req[INVALID_MESSAGE_ID] = (qint64)invalid_message_id;
    req[ENV_ID] = (qint64)env_id;
    this->m_sender->sendNetMessage(req);
}


// public slot
void NetworkHandler::connectToServer()
{
    this->m_socket->connectToHost(m_address, m_port);
}



// public slot
void NetworkHandler::handleNewNetMessage(const QJsonObject &net_msg)
{
    using namespace KeyWords;
    if (net_msg[NET_MESSAGE_TYPE] == REGISTER_RESULT
            || net_msg[NET_MESSAGE_TYPE] == LOGIN_RESULT)
    {
        emit this->entryNetMessageArrived(net_msg);
        if (net_msg[SUCCESSFUL].toBool())
            emit this->entrySuccessful(net_msg);
        return;
    }
    if (net_msg[NET_MESSAGE_TYPE] == DATA)
    {
        emit this->newDataArrived(net_msg);
        return;
    }
}

void NetworkHandler::sendEnvDetailsReq(const quint64& env_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = GET_PRIVATE_ENV_DETAILS;
    req[ENV_ID] = (qint64)env_id;
    this->m_sender->sendNetMessage(req);
}

void NetworkHandler::sendUserInfoReq(const quint64 &user_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = GET_USER_INFO;
    req[USER_ID] = (qint64)user_id;
    this->m_sender->sendNetMessage(req);
}
