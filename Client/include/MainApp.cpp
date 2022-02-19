#include "MainApp.hpp"


MainApp::MainApp(QQmlApplicationEngine& qml_engine,
                 QObject *parent,
                 const QString &address,
                 const quint16 port)
    : QObject(parent),
      m_network_handler(new NetworkHandler(this, address, port)),
      m_data_handler(new DataHandler(this, this->m_network_handler, this->m_user_info))
{
    QObject::connect(this->m_network_handler, &NetworkHandler::newDataArrived,
                     this->m_data_handler, &DataHandler::handleNewData, Qt::UniqueConnection);

    QObject::connect(this->m_network_handler, &NetworkHandler::entrySuccessful,
                     this, &MainApp::initiateApp, Qt::UniqueConnection);

    qml_engine.rootContext()->setContextProperty("messagesModel", this->m_data_handler->m_message_list_model);
    qml_engine.rootContext()->setContextProperty("chatsModel", this->m_data_handler->m_conversation_list_model);
    qml_engine.rootContext()->setContextProperty("netConn", this->m_network_handler);
    qml_engine.rootContext()->setContextProperty("dataHolder", this->m_data_handler);
    qml_engine.rootContext()->setContextProperty("db", this->m_data_handler->m_db);
}

void MainApp::initiateApp(const QJsonObject &net_msg)
{
    this->m_data_handler->m_db->convertToHash(this->m_user_info, net_msg[KeyWords::USER_INFO].toObject());
    this->m_data_handler->prepareDB();
    this->m_data_handler->fillConversationListModel();
    this->m_data_handler->registerAllPendingChats();
    this->m_data_handler->registerAllMessages();
}
