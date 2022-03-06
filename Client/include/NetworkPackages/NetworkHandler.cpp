#include "./NetworkHandler.hpp"
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QThread>

// constructor
NetworkHandler::NetworkHandler(QObject *parent,
                               const QString &address,
                               quint16 port)
    : QObject(parent),
      m_socket(new QTcpSocket),
      m_address(address),
      m_port(port),
      m_receiver(new NetMessageReceiver(this, this->m_socket)),
      m_sender(new NetMessageSender(this, this->m_socket))
{
    QObject::connect(this->m_socket, &QTcpSocket::disconnected,
                     this->m_receiver, &NetMessageReceiver::stopListening, Qt::UniqueConnection);

    QObject::connect(this->m_socket, &QTcpSocket::connected,
                     this->m_receiver, &NetMessageReceiver::startListening, Qt::UniqueConnection);

    QObject::connect(this->m_receiver, &NetMessageReceiver::newNetMessageArrived,
                     this, &NetworkHandler::handleNewNetMessage, Qt::UniqueConnection);

    QObject::connect(this->m_socket, &QTcpSocket::stateChanged,
                        [&](const QTcpSocket::SocketState& new_state)
                        {
                            qDebug() << new_state;
                            switch (new_state)
                            {
                                case QTcpSocket::ConnectedState:
                                    emit this->netConnectedChanged(true); break;
                                case QTcpSocket::ClosingState:
                                    emit this->netConnectedChanged(false); break;
                                default: break;
                            };
                        }
    );

    this->connectToServer();
}

void NetworkHandler::autoConnect(bool connected_now)
{
    if(!connected_now)
        while (this->m_socket->state() != QTcpSocket::ConnectedState)
        {
            qDebug() << "hereeee " << this->m_socket->state();
            QThread::usleep(500000);

                                this->connectToServer();
        }
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

void NetworkHandler::sendEnvDetailsReq(const quint64& env_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = GET_ENV_DETAILS;
    req [ENV_ID] = (qint64)env_id;
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


