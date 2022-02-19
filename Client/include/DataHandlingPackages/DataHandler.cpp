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
                  this, &DataHandler::fetchEnvDetails);
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

QString DataHandler::getProperConversationHeader(const quint64 &env_id,
                                                 const bool& is_pending)
{
    if(!is_pending && this->m_db->isValidPrivateChat(env_id))
        return this->m_db->getOtherAudienceNameInPrivateChat(env_id, false);
    else if(is_pending && this->m_db->isPendingPrivateChat(env_id))
        return this->m_db->getOtherAudienceNameInPrivateChat(env_id, true);

    qDebug() << "wasn't private!";
    return QString();
}

void DataHandler::validatePrivateChat(const NetInfoContainer &env_info)
{
    using namespace KeyWords;
    this->m_db->insertSinglePrivateEnv(env_info);
    this->m_db->deletePendingChat(env_info[INVALID_ENV_ID].toString().toULongLong());
    this->m_conversation_list_model->changeConversationToValid(env_info[INVALID_ENV_ID].toString().toULongLong());
}

// Q_INVOKABLE
void DataHandler::openPrivateChatWith(const quint64 user_id, const QString& name)
{
    using namespace KeyWords;
    InfoContainer chat_info;
    if(this->m_db->validPrivateChatAlreadyExists(user_id, chat_info))
    {
        this->openExistingChatEnv(chat_info[ENV_ID].toUInt(), false);
        return;
    }
    if(this->m_db->pendingPrivateChatAlreadyExists(user_id, chat_info))
    {
        this->openExistingChatEnv(chat_info[INVALID_ENV_ID].toUInt(), true);
        return;
    }
    // else: new pending private chat needs to be created

    this->createAndOpenNewPrivateChat(user_id, name);
    return;
}

void DataHandler::openExistingChatEnv(const quint64 &env_id, const bool &is_pending)
{
    this->feedEnvMessagesToMessagesModel(env_id, is_pending);
    this->m_conversation_list_model->selectChat(env_id, is_pending);
}

// Q_INVOKABLE
void DataHandler::sendNewTextMessage(const quint64 &env_id,
                                     const QString &message_text,
                                     const bool& is_pending)
{
    // ToDo
}

void DataHandler::createAndOpenNewPrivateChat(const quint64 &user_id, const QString &name)
{
    auto invalid_id = this->m_db->insertNewPendingPrivateChat(user_id);
    this->m_net_handler->sendCreateNewPrivateChatReq(user_id, invalid_id);
    this->m_conversation_list_model->appendConversation(
                {
                    invalid_id,
                    true,
                    name,
                    this->m_db->getMaxMessagesId()
                }
    );
    this->openExistingChatEnv(invalid_id, true);
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
void DataHandler::fetchEnvDetails(const quint64 &env_id)
{
    using namespace KeyWords;
    this->m_net_handler->sendEnvDetailsReq(env_id);
}

//private
void DataHandler::prepareDB()
{
    this->m_db->tryToInit();
    this->m_net_handler->sendFetchReq();
}


// private
void DataHandler::convertToHash(InfoContainer &target, const NetInfoContainer &source)
{
    for (const auto& key : source.keys())
        target[key] = source[key].toVariant();
}


