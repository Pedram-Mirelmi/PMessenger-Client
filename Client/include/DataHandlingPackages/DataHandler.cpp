#include "DataHandler.hpp"
#include <QMutex>
#include "../ClientKeywords.hpp"

DataHandler::DataHandler(QObject *parent, NetworkHandler *netHandler)
    :QObject(parent),
      m_db(new DataBase(this)),
      m_net_handler(netHandler),
      m_message_list_model(new MessageListModel(this)),
      m_conversation_list_model(new ConversationsListModel(this))
{
    this->connect(this->m_db, &DataBase::needPrivateEnvDetails,
                  this, &DataHandler::sendReqForPrivateEnvDetails);
    this->connect(this->m_db, &DataBase::newTextMessageInserted,
                  this->m_message_list_model, &MessageListModel::considerNewTextMessage);
    this->connect(this->m_db, &DataBase::newTextMessageInserted,
                  this->m_conversation_list_model, &ConversationsListModel::popUpConversation);
}

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
void DataHandler::prepareDB()
{
    this->m_db->tryToInit();
    this->m_net_handler->sendFetchReq();
}

void DataHandler::convertToHash(InfoContainer &target, const QJsonObject &source)
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}

// public slot
void DataHandler::handleNewData(const QJsonObject &net_message)
{
    using namespace KeyWords;
    const auto &data_type = net_message[DATA_TYPE];
    if (data_type == FETCH_RESULT)
    {
        this->handleFetchResult(net_message);
    }
    else if (data_type == ENV_DETAILS)
    {
        this->m_db->insertSinglePrivateEnv(net_message[ENV_INFO].toObject());
        this->m_db->insertTextMessages(net_message[TEXT_MESSAGES].toArray());
    }
    else if (data_type == MESSAGE)
    {
        if (net_message.contains(TEXT_MESSAGES))
            this->m_db->insertTextMessages(net_message[TEXT_MESSAGES].toArray());
    }
}

// private
void DataHandler::handleFetchResult(const QJsonObject &net_message)
{
    using namespace KeyWords;

    if (net_message.contains(PRIVATE_CHATS))
    {
        auto private_chats = net_message[PRIVATE_CHATS].toArray();
        for (const auto& private_env : private_chats)
            this->m_db->tryToInsertPrivateEnvs(private_env);
    }
}

// private
void DataHandler::sendReqForPrivateEnvDetails(const int &env_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = GET_PRIVATE_ENV_DETAILS;
    req [ENV_ID] = env_id;
    this->m_net_handler->m_sender->sendMessage(req);
}


// private

// private
