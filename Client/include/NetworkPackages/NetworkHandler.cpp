#include "./NetworkHandler.hpp"

// constructor
NetworkHandler::NetworkHandler(QObject *parent, const QString &address, quint16 port)
    :   QObject(parent),
      m_socket(new QTcpSocket),
      m_address(address),
      m_port(port),
      m_receiver(new NetMessageReceiver(this, this->m_socket)),
      m_sender(new NetMessageSender(this, this->m_socket))
{
    connect(this->m_socket, &QTcpSocket::disconnected, this->m_receiver, &NetMessageReceiver::stopListening);
    connect(this->m_socket, &QTcpSocket::connected, this->m_receiver, &NetMessageReceiver::startListening, Qt::UniqueConnection);
    connect(this->m_receiver, &NetMessageReceiver::newNetMessageArrived, this, &NetworkHandler::handleNewNetMessage);
    this->connectToServer();
    this->setAutoConnect(true);
}

// public:
void NetworkHandler::setAutoConnect(bool enable)
{
    if(enable)
        connect(this->m_socket, &QTcpSocket::disconnected, this, &NetworkHandler::connectToServer, Qt::UniqueConnection);
    else
        disconnect(this->m_socket, &QTcpSocket::disconnected, this, &NetworkHandler::connectToServer);
}


// Q_INVOKABLE
void NetworkHandler::sendRegisterReq(const QString &username, const QString &password)
{
    QJsonObject request;
    using namespace KeyWords;
    request[NET_MESSAGE_TYPE] = REGISTER;
    request[USERNAME] = username;
    request[PASSWORD] = password;
    this->m_sender->sendMessage(request);
    emit this->testSignal("Hiiiiii the fuuuckkkkkkkkkkkk");
}

// Q_INVOKABLE
void NetworkHandler::sendLoginReq(const QString &username, const QString &password)
{
    QJsonObject request;
    using namespace KeyWords;
    request[NET_MESSAGE_TYPE] = LOGIN;
    request[USERNAME] = username;
    request[PASSWORD] = password;
    this->m_sender->sendMessage(request);
}

// Q_INVOKSBLE
void NetworkHandler::sendFetchReq()
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = FETCH;
    this->m_sender->sendMessage(req);
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
    if (net_msg[NET_MESSAGE_TYPE] == REGISTER_RESULT || net_msg[NET_MESSAGE_TYPE] == LOGIN_RESULT)
    {
        emit this->entryNetMessageArrived(net_msg);
        return;
    }
    if (net_msg[NET_MESSAGE_TYPE] == USER_SEARCH_RESULT)
    {
        emit this->userSearchResultArrived(net_msg);
        return;
    }
    else if (net_msg[NET_MESSAGE_TYPE] == CHAT_CREATION_CONFIRMATION)
    {
        emit this->newChatCreationMsgArrived(net_msg);
        return;
    }
    if (net_msg[NET_MESSAGE_TYPE] == DATA)
    {
        emit this->newDataArrived(net_msg);
        return;
    }
    if (net_msg[NET_MESSAGE_TYPE] == USER_SEARCH_RESULT)
    {
        emit this->userSearchResultArrived(net_msg);
        return;
    }

}















