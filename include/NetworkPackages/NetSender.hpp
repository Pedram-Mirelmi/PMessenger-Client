#pragma once
#include <QDebug>
#include <thread>
#include <QString>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "../../KeyWords.hpp"
#include <cmath>

class NetMessageSender : public QObject
{
private:
    QTcpSocket* m_socket;
public:

    explicit NetMessageSender(QObject* parent, QTcpSocket* socket);
    void sendNetMessage(const QJsonObject& message);
};
