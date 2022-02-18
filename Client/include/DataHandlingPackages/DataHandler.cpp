#include "DataHandler.hpp"
#include <QMutex>
#include <QDataStream>
#include <QHash>
#include <QFile>
#include "../ClientKeywords.hpp"

DataHandler::DataHandler(QObject *parent, NetworkHandler *netHandler, InfoContainer& user_info)
    : QObject(parent),
      m_user_info(user_info),
      m_db(new DataBase(this, this->m_user_info)),
      m_net_handler(netHandler),
      m_message_list_model(new MessageListModel(this)),
      m_conversation_list_model(new ConversationsListModel(this))
{
    this->connect(this->m_db, &DataBase::needPrivateEnvDetails,
                  this, &DataHandler::fetchPrivateEnvDetails);
    this->connect(this->m_db, &DataBase::newTextMessageInserted,
                  this->m_message_list_model, &MessageListModel::considerNewTextMessage);
    this->connect(this->m_db, &DataBase::newTextMessageInserted,
                  this->m_conversation_list_model, &ConversationsListModel::popUpConversation);
}

void DataHandler::feedEnvMessagesToMessagesModel(const quint64 &env_id,
                                                 const bool& is_pending_env)
{
    this->m_message_list_model->beginResetModel();
    this->m_message_list_model->m_messages.clear();
    this->m_db->getEnvTextMessages(env_id, this->m_message_list_model->m_messages, is_pending_env);
    this->m_message_list_model->endResetModel();
}

void DataHandler::validatePrivateChat(const NetInfoContainer &env_info)
{
    using namespace KeyWords;
    this->m_db->insertSinglePrivateEnv(env_info);
    this->m_db->deletePendingChat(env_info[INVALID_ENV_ID].toString().toULongLong());
    qDebug() << env_info[INVALID_ENV_ID];
    this->m_conversation_list_model->changeConversationToValid(env_info[INVALID_ENV_ID].toString().toULongLong());
}

// Q_INVOKABLE
void DataHandler::openPrivateChatWith(const quint64 user_id, const QString& name)
{
    using namespace KeyWords;
    InfoContainer chat_info;
    this->m_db->getPrivateChatInfo(user_id, chat_info);
    if(chat_info.isEmpty()) // there is not any valid private chat with user_id
    {
        bool newly_created = false;
        this->m_db->getPendingPrivateChatInfo(user_id, chat_info);
        if (chat_info.isEmpty()) // there is not any pending private chat with user_id
        {   // create pending private chat
            newly_created = true;
            auto invalid_id = this->m_db->insertNewPendingPrivateChat(user_id);
            this->m_net_handler->sendCreateNewPrivateChatReq(user_id, invalid_id);
            this->m_db->getPendingPrivateChatInfo(invalid_id, chat_info);
        }
        this->openPendingPrivateChat(chat_info, name, newly_created);
        return;
    }
    this->feedEnvMessagesToMessagesModel(chat_info[ENV_ID].toUInt(), false);
}

// Q_INVOKABLE
void DataHandler::sendNewTextMessage(const quint64 &env_id,
                                     const QString &message_text)
{
    auto invalid_inserted_id = this->m_db->insertPendingTextMessage(env_id, message_text, true);
    this->m_net_handler->sendNewTextMessageReq(env_id, message_text, invalid_inserted_id);
}

void DataHandler::saveUser(const quint64 &user_id,
                           const QString &username,
                           const QString &name)
{
    this->m_db->tryToInsertUser(user_id, username, name);
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
        this->m_db->insertValidTextMessages(net_message[TEXT_MESSAGES].toArray());
    }
    else if (data_type == MESSAGE)
    {
        if (net_message.contains(TEXT_MESSAGES))
            this->m_db->insertValidTextMessages(net_message[TEXT_MESSAGES].toArray());
    }
    else if (data_type == CHAT_CREATION_CONFIRMATION)
    {
        if(net_message[ENV_INFO][ENV_TYPE] == PRIVATE_CHAT)
            this->validatePrivateChat(net_message[ENV_INFO].toObject());
    }
}

void DataHandler::fillConversationListModel()
{
    using namespace KeyWords;
    this->m_conversation_list_model->beginResetModel();
    QVector<InfoContainer> all_conversations;
    this->m_db->getAllRegisteredEnvs(all_conversations);
    auto number_of_registered_envs = all_conversations.size();
    this->m_db->getAllPendingEnvs(all_conversations);
    quint16 i = 0;
    auto& new_data = this->m_conversation_list_model->m_conversations;
    new_data.reserve(all_conversations.size());
    for (; i < number_of_registered_envs; i++)
        new_data.emplace_back(
                    all_conversations[i][ENV_ID].toUInt(),
                    false,
                    ((all_conversations[i][FIRST_PERSON].toUInt() == this->m_user_info[USER_ID].toUInt()) ?
                        this->m_db->getNameOfUser(all_conversations[i][FIRST_PERSON].toUInt()) :
                        this->m_db->getNameOfUser(all_conversations[i][SECOND_PERSON].toUInt())),
                    this->m_db->getLastEnvMessageId(all_conversations[i][ENV_ID].toUInt())
                 );
    for (; i < all_conversations.size(); i++)
        new_data.emplace_back(
                    all_conversations[i][INVALID_ENV_ID].toUInt(),
                    true,
                    (all_conversations[i][FIRST_PERSON].toUInt() == this->m_user_info[USER_ID].toUInt()) ?
                        this->m_db->getNameOfUser(all_conversations[i][FIRST_PERSON].toUInt()) :
                        this->m_db->getNameOfUser(all_conversations[i][SECOND_PERSON].toUInt()),
                    this->m_db->getMaxMessagesId()
                 );
    this->m_conversation_list_model->endResetModel();
}

void DataHandler::registerAllPendingChats()
{
    using namespace KeyWords;
    QVector<InfoContainer> envs_infos;
    this->m_db->getAllPendingEnvs(envs_infos);
    for(const auto& env_info : envs_infos)
    {
        if(env_info[ENV_TYPE].toString() == PRIVATE_CHAT)
        {
            this->m_net_handler->sendCreateNewPrivateChatReq(env_info[SECOND_PERSON].toUInt(),
                                                             env_info[INVALID_ENV_ID].toUInt());
        }
    }
}

void DataHandler::registerAllMessages()
{

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
void DataHandler::fetchPrivateEnvDetails(const quint64 &env_id)
{
    using namespace KeyWords;
    this->m_net_handler->sendPrivateEnvDetailsReq(env_id);
}

//private
void DataHandler::prepareDB()
{
    this->m_db->tryToInit();
    this->m_net_handler->sendFetchReq();
}


void DataHandler::openPendingPrivateChat(const InfoContainer& chat_info,
                                         const QString& name,
                                         const bool& newly_created)
{
    using namespace KeyWords;
    if (newly_created)
        this->m_conversation_list_model->tryToAppendConversation({
                                                                     chat_info[INVALID_ENV_ID].toUInt(),
                                                                     true,
                                                                     name,
                                                                     this->m_db->getMaxMessagesId()
                                                                 });
    feedEnvMessagesToMessagesModel(chat_info[INVALID_ENV_ID].toUInt(), true);
}


// private
void DataHandler::convertToHash(InfoContainer &target, const QJsonObject &source)
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}


