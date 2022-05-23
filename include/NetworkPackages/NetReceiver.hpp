#pragma once
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTcpSocket>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonObject>
#include "../../KeyWords.hpp"

class NetMessageReceiver : public QObject
{
    Q_OBJECT
    QTcpSocket* m_socket;
    quint8 m_header_size;
    QByteArray m_inbuff;
    QMutex m_lock;
    bool m_header_has_been_read = false;
    qsizetype m_current_header;
public:
    explicit NetMessageReceiver(QObject* parent, QTcpSocket* socket, const quint8& header_size = 8);

public slots:
//    void testSlot()
//    {
//        qDebug() << "hereee:////////";
//    }
    void startListening();
    void stopListening();

private slots:
    void receiveMessage ();

private:
    void tryToParsMessage();

signals:
   void newNetMessageArrived(const QJsonObject& msg);
};
