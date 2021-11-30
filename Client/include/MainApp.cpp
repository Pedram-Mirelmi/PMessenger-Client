#include "MainApp.hpp"

MainApp::MainApp(QObject *parent, const QString &address, const quint16 port)
    : QObject(parent),
      m_network_handler(new NetworkHandler(this, address, port)),
      m_data_handler(new DataHandler(this, this->m_network_handler))
{
    qDebug() << "in main" ;
    this->m_network_handler->connectToServer();
    this->m_network_handler->setAutoConnect(true);
    connect(this->m_network_handler->m_receiver, &NetMessageReceiver::newNetMessageArrived,
            this, &MainApp::handleNetMessage);
}

void MainApp::sendRegisterReq(const QString &username, const QString &password)
{
    QJsonObject request;
    using namespace KeyWords;
    request[MESSAGE_TYPE] = REGISTER;
    request[USERNAME] = username;
    request[PASSWORD] = password;
    this->m_network_handler->m_sender->sendMessage(request);
}

void MainApp::sendLoginReq(const QString &username, const QString &password)
{
    QJsonObject request;
    using namespace KeyWords;
    request[MESSAGE_TYPE] = LOGIN;
    request[USERNAME] = username;
    request[PASSWORD] = password;
    this->m_network_handler->m_sender->sendMessage(request);
}

void MainApp::handleNetMessage(QJsonObject msg)
{
    using namespace KeyWords;
    if(msg[MESSAGE_TYPE].toString() == ENTERING)
        emit enterResponseCaugth(msg[OUTCOME].toBool(), msg[DETAILS].toString());

}
