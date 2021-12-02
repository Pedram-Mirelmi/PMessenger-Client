#include "MainApp.hpp"

MainApp::MainApp(QObject *parent, const QString &address, const quint16 port)
    : QObject(parent),
      m_network_handler(new NetworkHandler(this, address, port)),
      m_data_handler(new DataHandler(this, this->m_network_handler))
{
    qDebug() << "in main";
    QObject::connect(this->m_network_handler, &NetworkHandler::newDataArrived,
                     this->m_data_handler, &DataHandler::handleNewData, Qt::UniqueConnection);
    QObject::connect(this->m_network_handler, &NetworkHandler::entryNetMessageArrived,
                     this, &MainApp::initiateDb, Qt::UniqueConnection);

}























