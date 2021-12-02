#include "DataHandler.hpp"
#include "../Others/format.hpp"

DataHandler::DataHandler(QObject *parent, NetworkHandler *netHandler)
    :QObject(parent), m_net_handler(netHandler)
{}

//public
void DataHandler::startDB()
{
    if (this->m_db.tryToInit())
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
            insertPrivateMessages(net_message[PRIVATE_MESSAGES].toArray());
        if (net_message.contains(GROUP_MESSAGES))
            insertGroupMessages(net_message[GROUP_MESSAGES].toArray());
        if(net_message.contains(CHANNEL_MESSAGES))
            insertChannelMessages(net_message[CHANNEL_MESSAGES].toArray());
    }
}

// private
void DataHandler::handleFetchAllResult(const QJsonObject &net_message)
{
    using namespace KeyWords;
    if (net_message.contains(PRIVATE_CHATS))
        for (const auto& private_env : net_message[PRIVATE_CHATS].toArray())
            this->insertPrivateEnv(private_env.toObject());
    if (net_message.contains(GROUP_CHATS))
        for (const auto& group_env : net_message[GROUP_CHATS].toArray())
            this->insertGroupEnv(group_env.toObject());
    if (net_message.contains(CHANNEL_MESSAGES))
        for (const auto& channel_env : net_message[CHANNELS].toArray())
            this->insertChannelEnv(channel_env.toObject());
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
bool DataHandler::insertPrivateEnv(const QJsonObject &env)
{
    using namespace KeyWords;
    auto general_env_insert_Qry = string_format("INSERT INTO chat_envs(env_id, participates) VALUES(%d, 1);", env[ENV_ID].toInt());
    auto private_env_insert_Qry = string_format("INSERT INTO private_chats(env_id, first_person, second_person) VALUES(%d, %d, %d);",
                                                env[ENV_ID].toInt(), env[FIRST_PERSON].toInt(), env[SECOND_PERSON].toInt());
    this->m_db.execOtherQry(general_env_insert_Qry.c_str());
    return this->m_db.execOtherQry(private_env_insert_Qry.c_str());
}

// private
bool DataHandler::insertGroupEnv(const QJsonObject &env)
{
    // TODO
}

// private
bool DataHandler::insertChannelEnv(const QJsonObject &env)
{
    // TODO
}

// private
void DataHandler::insertGroupMessages(const QJsonArray &messages)
{
    // TODO
}

// private
void DataHandler::insertChannelMessages(const QJsonArray &messages)
{
    // TODO
}

// private
void DataHandler::insertPrivateMessages(const QJsonArray &messages)
{
    using namespace KeyWords;
    bool ok;
    for (const auto& message : this->convertToNormalForm(messages))
    {
        auto msg_obj = message.toObject();
        auto general_message_insert_query = string_format(
                    "INSERT INTO messages(message_id, owner_id, env_id, created_at) VALUES "
                    "                     (%d,        %d,       %d,     '%s' )",
                    msg_obj[MESSAGE_ID].toInt(), msg_obj[OWNER_ID].toInt(), msg_obj[ENV_ID].toInt(), msg_obj[CREATED_AT].toString());
        ok = this->m_db.execOtherQry(general_message_insert_query.c_str());
        auto text_message_insert_query = string_format(
                        "INSERT INTO text_messages(message_id, message_text) VALUES "
                        "                          (%d          &s)",
                                                   msg_obj[MESSAGE_ID].toInt(), msg_obj[MESSAGE_TEXT].toString());
        ok = this->m_db.execOtherQry(text_message_insert_query.c_str()) && ok;
        if (msg_obj.contains(SEEN) && !msg_obj[SEEN].toBool() && ok)
        {
            emit newMessageReceived(msg_obj);
        }
    }
}

// private
QJsonArray DataHandler::convertToNormalForm(const QJsonArray &data)
{
    QJsonArray result;
    auto fields = data[0].toArray();
    for (auto itter = data.constBegin() + 1; itter < data.constEnd(); itter++)
    {
        QJsonObject new_row;
        const QJsonArray& data_row = itter->toArray();
        for (uint8_t i = 0; i<fields.size(); i++)
        {
            new_row[fields[i].toString()] = data_row[i];
        }
    }
    return result;
}
