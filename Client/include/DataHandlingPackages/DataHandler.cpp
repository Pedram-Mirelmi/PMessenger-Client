#include "DataHandler.hpp"
#include <QMutex>
#include "../ClientKeywords.hpp"

DataHandler::DataHandler(QObject *parent, NetworkHandler *netHandler)
    :QObject(parent),
      m_db(new DataBase(this)),
      m_net_handler(netHandler),
      m_message_list_model(new MessageListModel(this)),
      m_conversation_list_model(new ConversationsListModel(this))
{}

void DataHandler::feedNewMessagesToModel(const int &env_id)
{
    this->m_message_list_model->m_messages.clear();
    this->m_message_list_model->clearModel();
    // TODO -- needs to be corrected when added new message types features
    this->m_message_list_model->beginResetModel();
    this->m_db->SELECT(this->m_message_list_model->m_messages,
                       fmt::format("SELECT * FROM text_messages_view WHERE env_id={};", env_id).data());
    this->m_message_list_model->endResetModel();
}

//public
void DataHandler::startDB()
{
    if (this->m_db->tryToInit()) // if inited
        this->m_net_handler->sendFetchAllReq();
}

// public slot
void DataHandler::handleNewData(const QJsonObject &net_message)
{
    using namespace KeyWords;
    auto data_type = net_message[DATA_TYPE];
    if (data_type == FETCH_ALL_RESULT)
    {
        this->handleFetchAllResult(net_message);
    }
    else if (data_type == FETCH_RESULT)
    {

    }
    else if (data_type == MESSAGE)
    {
        if (net_message.contains(PRIVATE_MESSAGES))
            this->m_db->insertPrivateMessages(net_message[PRIVATE_MESSAGES].toArray());
        if (net_message.contains(GROUP_MESSAGES))
            this->m_db->insertGroupMessages(net_message[GROUP_MESSAGES].toArray());
        if(net_message.contains(CHANNEL_MESSAGES))
            this->m_db->insertChannelMessages(net_message[CHANNEL_MESSAGES].toArray());
    }
}

// private
void DataHandler::handleFetchAllResult(const QJsonObject &net_message)
{
    using namespace KeyWords;

    if (net_message.contains(PRIVATE_CHATS))
    {
        auto private_chats = net_message[PRIVATE_CHATS].toArray();
        for (const auto& private_env : private_chats)
            this->m_db->insertPrivateEnv(private_env.toObject());
    }
    if (net_message.contains(GROUP_CHATS))
    {
        auto group_chats = net_message[GROUP_CHATS].toArray();
        for (const auto& group_env : group_chats)
            this->m_db->insertGroupEnv(group_env.toObject());
    }
    if (net_message.contains(CHANNEL_MESSAGES))
    {
        auto channels = net_message[CHANNELS].toArray();
        for (const auto& channel_env : channels)
            this->m_db->insertChannelEnv(channel_env.toObject());
    }
}

// private
void DataHandler::sendReqForChatEnvMessages(const int &env_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = GET_ENV_MESSAGES;
    req [ENV_ID] = env_id;
    this->m_net_handler->m_sender->sendMessage(req);
}


// private

// private
