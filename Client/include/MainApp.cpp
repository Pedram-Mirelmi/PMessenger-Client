#include "MainApp.hpp"

MainApp::MainApp(QQmlApplicationEngine& qml_engin, QObject *parent, const QString &address, const quint16 port)
    : QObject(parent),
      m_network_handler(new NetworkHandler(this, address, port)),
      m_data_handler(new DataHandler(this, this->m_network_handler))
{
    qDebug() << "in main";
    QObject::connect(this->m_network_handler, &NetworkHandler::newDataArrived,
                     this->m_data_handler, &DataHandler::handleNewData, Qt::UniqueConnection);
    QObject::connect(this->m_network_handler, &NetworkHandler::entryNetMessageArrived,
                     this, &MainApp::initiateDb, Qt::UniqueConnection);
    qml_engin.rootContext()->setContextProperty("messages_model", this->m_data_handler->m_message_list_model);
    qml_engin.rootContext()->setContextProperty("chats_model", this->m_data_handler->m_conversation_list_model);
    qml_engin.rootContext()->setContextProperty("net", this->m_network_handler);
}

void MainApp::initiateDb(const QJsonObject &net_msg)
{
    using namespace KeyWords;
    if (net_msg[SUCCESSFUL].toBool())
        this->m_data_handler->prepareDB();
}























