#include "DataHandler.hpp"
#include "../ClientKeywords.hpp"
#include <QMutex>
//#include <mutex>
QMutex modelLock; // for all_chats

// plays the main role for model
static QJsonObject all_chats;
/*
    This form:
    {
        env_id: {
            <info>
        },

        env_id: {
            <info>
        },
        .
        .
        .
    }

*/

DataHandler::DataHandler(QObject *parent, NetworkHandler *netHandler)
    :QObject(parent), m_db(new DataBase(this)), m_net_handler(netHandler)
{}

//public
void DataHandler::startDB()
{
    if (this->m_db->tryToInit())
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

void DataHandler::addNewMessageToModel(const QJsonObject &msg_info)
{
    modelLock.lock();
    using namespace KeyWords;
    if (!all_chats.contains(msg_info[ENV_ID].toString()))
    {
        // ERROR
        return;
    }
    all_chats[msg_info[ENV_ID].toString()].toArray().append(msg_info);
    auto current_index = all_chats[msg_info[ENV_ID].toString()].toArray()

    modelLock.unlock();
}

void DataHandler::addNewChatEnvToModel(const QJsonObject &env_info)
{

}

// private
void DataHandler::handleFetchAllResult(const QJsonObject &net_message)
{
    using namespace KeyWords;
    if (net_message.contains(PRIVATE_CHATS))
        for (const auto& private_env : net_message[PRIVATE_CHATS].toArray())
            this->m_db->insertPrivateEnv(private_env.toObject());
    if (net_message.contains(GROUP_CHATS))
        for (const auto& group_env : net_message[GROUP_CHATS].toArray())
            this->m_db->insertGroupEnv(group_env.toObject());
    if (net_message.contains(CHANNEL_MESSAGES))
        for (const auto& channel_env : net_message[CHANNELS].toArray())
            this->m_db->insertChannelEnv(channel_env.toObject());
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
