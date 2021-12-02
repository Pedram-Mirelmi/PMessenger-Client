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


// public slot
void NetworkHandler::connectToServer()
{
    this->m_socket->connectToHost(m_address, m_port);
}


// public slot
void NetworkHandler::handleNewNetMessage(const QJsonObject &net_msg)
{
    using namespace KeyWords;
    if (net_msg[MESSAGE_TYPE] == REGISTER || net_msg[MESSAGE_TYPE] == LOGIN)
        emit entryNetMessageArrived(net_msg);
    else if (net_msg[MESSAGE_TYPE] == USER_SEARCH_RESULT)
        emit userSearchResultArrived(net_msg);
    else if (net_msg[MESSAGE_TYPE] == CHAT_CREATION_CONFIRMATION)
        emit newChatCreationMsgArrived(net_msg);
    else if (net_msg[MESSAGE_TYPE] == MESSAGE_SENT_CONFIRMATION)
        emit messageConfirmationArrived(net_msg);
    else if (net_msg[MESSAGE_TYPE] == DATA)
        emit newDataArrived(net_msg);
}















