#include "MainApp.hpp"


MainApp::MainApp(QQmlApplicationEngine& qml_engine,
                 QObject *parent,
                 const QString &address,
                 const quint16 port)
    : QObject(parent),
      m_network_handler(new NetworkHandler(nullptr, address, port)),
      m_data_handler(new DataHandler(nullptr, this->m_network_handler, this->m_user_info))
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

// Q_INVOKABLE
void MainApp::openPrivateChatWith(const quint64 user_id, const QString &name)
{
    using namespace KeyWords;
    InfoContainer chat_info;
    if(this->m_data_handler->m_db->setValidPrivateChatInfoIfExists(user_id, chat_info))
    {
        this->openExistingChatEnv(chat_info[ENV_ID].toUInt(), false);
        return;
    }
    if(this->m_data_handler->m_db->setInvalidPrivateChatInfoIfExists(user_id, chat_info))
    {
        this->openExistingChatEnv(chat_info[INVALID_ENV_ID].toUInt(), true);
        return;
    }
    // else: new pending private chat needs to be created
    auto invalid_id = this->m_data_handler->createNewPrivateChat(user_id, name);
    this->openExistingChatEnv(invalid_id, true);
}

// Q_INVOKABLE
void MainApp::openExistingChatEnv(const quint64 &env_id, const bool &is_pending)
{
    this->m_data_handler->feedEnvMessagesToMessagesModel(env_id, is_pending);
    this->m_data_handler->m_conversation_list_model->selectChat(env_id, is_pending);
}

// Q_INVOKABLE
void MainApp::sendNewTextMessage(const quint64 &env_id,
                                 const QString &message_text,
                                 const bool &to_pending)
{

}

void MainApp::initiateApp(const QJsonObject &net_msg)
{
    this->m_data_handler->m_db->convertToHash(this->m_user_info, net_msg[KeyWords::USER_INFO].toObject());
    this->m_data_handler->prepareDB();
    this->m_data_handler->fillConversationListModel();
    this->m_data_handler->registerAllPendingChats();
    this->m_data_handler->registerAllMessages();
}
