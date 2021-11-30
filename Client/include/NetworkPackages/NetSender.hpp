#pragma once
#include <QDebug>
#include "../Commons/KeyWords.h"
#include <thread>
#include <QString>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "../../../Commons/KeyWords.h"
#include <cmath>

class NetMessageSender : public QObject
{
private:
    QTcpSocket* m_socket;
public:

    explicit NetMessageSender(QObject* parent, QTcpSocket* socket);
    void sendMessage(const QJsonObject& message);
};
