#include "./NetSender.hpp"



NetMessageSender::NetMessageSender(QObject *parent, QTcpSocket *socket)
    : QObject(parent), m_socket(socket)
{}

void NetMessageSender::sendMessage(const QJsonObject &message)
{
    QByteArray message_string = QJsonDocument(message).toJson(QJsonDocument::Compact);
    QByteArray message_header(
                8 - ceil(log10(message_string.length())),
                ' ');
    message_header.append(QString::number(message_string.length()).toUtf8());
    this->m_socket->write(message_header);
    this->m_socket->write(message_string);
}
