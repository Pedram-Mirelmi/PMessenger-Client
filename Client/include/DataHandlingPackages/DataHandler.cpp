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
    QObject::connect(this->m_db, &DataBase::needPrivateEnvDetails,
                        [&](const quint64 &env_id)
                        {
                            this->m_net_handler->sendEnvDetailsReq(env_id);
                        }
    );

    QObject::connect(this->m_db, &DataBase::newValidTextMessageInserted,
                     this->m_message_list_model, &MessageListModel::considerNewTextMessage);

    QObject::connect(this->m_db, &DataBase::newValidTextMessageInserted,
                     this->m_conversation_list_model, &ConversationsListModel::popUpConversation);
}


//private
void DataHandler::prepareDB()
{
    this->m_db->tryToInit();
    this->m_net_handler->sendFetchReq();
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
        this->m_db->insertValidPrivateEnv(net_message[ENV_INFO].toObject());
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
    else if (data_type == SEARCH_USERNAME_RESULT)
    {
        auto search_results = this->m_db->convertToNormalForm(net_message[SEARCH_RESULT].toArray());
        emit this->searchUsernameResultArrived(*search_results);
    }
}


void DataHandler::registerAllPendingChats()
{
    using namespace KeyWords;

    auto pendings = this->m_db->getAllPendingEnvs();
    for(const auto& env_info : (*pendings))
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
        this->m_db->tryToInsertPrivateEnvs(net_message[PRIVATE_CHATS].toArray());
    }
}


quint64 DataHandler::createNewPrivateChat(const quint64 &user_id, const QString &name)
{
    auto invalid_id = this->m_db->insertPendingPrivateChat(user_id);
    this->m_net_handler->sendCreateNewPrivateChatReq(user_id, invalid_id);
    this->m_conversation_list_model->appendConversation(
                {
                    invalid_id,
                    true,
                    name,
                    this->m_db->getMaxMessagesId()
                }
                );
    return invalid_id;
}


void DataHandler::validatePrivateChat(const NetInfoContainer &env_info)
{
    using namespace KeyWords;
    this->m_db->insertValidPrivateEnv(env_info);
    this->m_db->deletePendingChat(env_info[INVALID_ENV_ID].toInteger());
    this->m_conversation_list_model->changeConversationToValid(env_info[INVALID_ENV_ID].toInteger());
}



void DataHandler::feedEnvMessagesToMessagesModel(const quint64 &env_id,
                                                 const bool& is_pending_env)
{
    this->m_message_list_model->beginResetModel();
    this->m_message_list_model->m_messages->clear();
    this->m_message_list_model->m_messages = this->m_db->getEnvTextMessages(env_id, is_pending_env);
    this->m_message_list_model->endResetModel();
}


void DataHandler::fillConversationListModel()
{
    using namespace KeyWords;
    this->m_conversation_list_model->beginResetModel();
    auto registereds = this->m_db->getAllRegisteredEnvs();
    auto number_of_registered_envs = registereds->size();
    auto pendings = this->m_db->getAllPendingEnvs();
    quint16 i = 0;
    auto& new_data = this->m_conversation_list_model->m_conversations;
    new_data.clear();
    new_data.reserve(registereds->size() + pendings->size());

    for (; i < number_of_registered_envs; i++)
        new_data.emplace_back(
                    pendings->value(i)[ENV_ID].toUInt(),
                    false,
                    ((registereds->value(i)[FIRST_PERSON].toUInt() == this->m_user_info[USER_ID].toUInt()) ?
                         this->m_db->getNameOfUser(registereds->value(i)[FIRST_PERSON].toUInt()) :
                         this->m_db->getNameOfUser(registereds->value(i)[SECOND_PERSON].toUInt())),
                    this->m_db->getLastEnvMessageId(registereds->value(i)[ENV_ID].toUInt())
                    );

    for (; i < new_data.capacity(); i++)
        new_data.emplace_back(
                    pendings->value(i)[INVALID_ENV_ID].toUInt(),
                    true,
                    (pendings->value(i)[FIRST_PERSON].toUInt() == this->m_user_info[USER_ID].toUInt()) ?
                        this->m_db->getNameOfUser(pendings->value(i)[FIRST_PERSON].toUInt()) :
                        this->m_db->getNameOfUser(pendings->value(i)[SECOND_PERSON].toUInt()),
                    this->m_db->getMaxMessagesId()
                    );

    this->m_conversation_list_model->endResetModel();
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

