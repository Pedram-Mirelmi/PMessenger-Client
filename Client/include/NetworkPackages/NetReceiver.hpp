#pragma once
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTcpSocket>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonObject>
#include "../../../Commons/KeyWords.h"

class NetMessageReceiver : public QObject
{
    Q_OBJECT
    QTcpSocket* m_socket;
    quint8 m_header_size;
    QByteArray m_inbuff;
    QMutex m_lock;
    bool m_header_has_been_read = false;
    quint8 m_current_header;
public:
    explicit NetMessageReceiver(QObject* parent, QTcpSocket* socket, const quint8& header_size = 8);

public slots:
    void startListening();
    void stopListening();

private slots:

    void receiveMessage ();

private:
    void tryToParsMessage();

signals:
   void newNetMessageArrived(QJsonObject msg);
};
