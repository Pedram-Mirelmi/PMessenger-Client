#include "./NetReceiver.hpp"


NetMessageReceiver::NetMessageReceiver(QObject *parent, QTcpSocket *socket, const quint8 &header_size)
    : QObject(parent),
      m_socket(socket),
      m_header_size(header_size)
{}

void NetMessageReceiver::startListening()
{
    connect(this->m_socket, &QTcpSocket::readyRead, this, &NetMessageReceiver::receiveMessage, Qt::QueuedConnection);
}

void NetMessageReceiver::stopListening()
{
    disconnect(this->m_socket, &QTcpSocket::readyRead, this, &NetMessageReceiver::receiveMessage);
}

void NetMessageReceiver::receiveMessage()
{
    m_lock.lock();
    this->m_inbuff.append(this->m_socket->readAll());
    this->tryToParsMessage();
    m_lock.unlock();
}

void NetMessageReceiver::tryToParsMessage()
{
    if ((!m_header_has_been_read) && this->m_inbuff.size() >= m_header_size)
    {
        this->m_current_header = m_inbuff.first(m_header_size).toInt();
        this->m_inbuff.remove(0, m_header_size);
        this->m_header_has_been_read = true;
    }
    if ((m_header_has_been_read) && (this->m_inbuff.size() >= m_current_header))
    {
//        auto new_net_message = QJsonDocument::fromJson(m_inbuff.first(m_current_header)).object();
        emit this->newNetMessageArrived(QJsonDocument::fromJson(m_inbuff.first(m_current_header)).object());
        this->m_inbuff.remove(0, m_current_header);
        this->m_header_has_been_read = false;
        tryToParsMessage();
    }
}
