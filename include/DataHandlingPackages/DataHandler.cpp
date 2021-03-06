#include "DataHandler.hpp"
#include <QMutex>
#include <QDataStream>
#include <QHash>
#include <QFile>
#include "../ClientKeywords.hpp"
#include "../Others/CommonTools.hpp"

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

    QObject::connect(this->m_db, &DataBase::needUserInfo,[=](const quint64& user_id)
                        {
                            this->m_net_handler->sendUserInfoReq(user_id);
                        }
    );

    QObject::connect(this->m_db, &DataBase::needPrivateEnvMessages, [=](const quint64& env_id, const quint64& last_message_we_have)
                        {
                            this->m_net_handler->sendEnvMessagesReq(env_id, last_message_we_have);
                        }
    );

    QObject::connect(this->m_db, &DataBase::newValidTextMessageInserted,
                     this->m_message_list_model, &MessageListModel::considerNewTextMessage);

    QObject::connect(this->m_db, &DataBase::newValidTextMessageInserted,
                     this->m_conversation_list_model, &ConversationsListModel::considerNewTextMessage);

//    QObject::connect(this->m_db, &DataBase::newValidPrivateEnvInserted,
//                     this->m_conversation_list_model, &ConversationsListModel::addValidatePrivateEnv);

    QObject::connect(this, &DataHandler::newPrivateEnvValidated,
                     this->m_conversation_list_model, &ConversationsListModel::considerValidatedPrivateEnv);


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
    else if (data_type == PRIVATE_ENV_DETAILS)
    {
        this->m_db->tryToInsertUser(net_message[OTHER_PERSON_INFO].toObject());
        this->m_db->insertValidPrivateEnv(convertToHash(net_message[ENV_INFO].toObject()));
        this->m_conversation_list_model->insertEnv(ConversatonItem(net_message[ENV_INFO][ENV_ID].toInteger(),
                                                                   false,
                                                                   net_message[OTHER_PERSON_INFO][NAME].toString(),
                                                                   this->m_db->getLastEnvMessageId(net_message[ENV_INFO][ENV_ID].toInteger())));
    }
    else if (data_type == MESSAGE)
    {
        if (net_message.contains(TEXT_MESSAGES))
            this->m_db->insertValidTextMessagesList(net_message[TEXT_MESSAGES].toArray());
    }
    else if (data_type == USER_INFO)
    {
        this->m_db->tryToInsertUser(net_message[USER_INFO].toObject());
    }
    else
        qDebug() << "Undefined received data type: " << data_type;
}


void DataHandler::registerAllPendingChats()
{
    using namespace KeyWords;
    auto pendings = this->m_db->getAllPendingEnvs();
    for(const auto& env_info : (*pendings))
    {
        if(env_info[ENV_TYPE].toString() == PRIVATE_CHAT)
        {
            this->m_net_handler->sendCreateNewPrivateChatReq(env_info[OTHER_PERSON_ID].toUInt(),
                                                             env_info[INVALID_ENV_ID].toUInt());
        }
    }
}

void DataHandler::registerAllPendingMessages()
{

}

void DataHandler::validatePrivateChat(const InfoContainer &valid_env_info)
{
    using namespace KeyWords;
    this->m_db->insertValidPrivateEnv(valid_env_info);
    this->m_db->deletePendingChat(valid_env_info[INVALID_ENV_ID].toUInt());
    this->m_conversation_list_model->changeConversationToValid(valid_env_info[INVALID_ENV_ID].toUInt(),
                                                               valid_env_info[ENV_ID].toUInt());
    this->newPrivateEnvValidated(valid_env_info[INVALID_ENV_ID].toUInt(), valid_env_info[ENV_ID].toUInt());
}

void DataHandler::validateTextMessage(const InfoContainer &valid_message_info)
{
    using namespace KeyWords;
    this->m_db->insertValidTextMessage(valid_message_info);
    this->m_db->deletePendingTextMessage(valid_message_info[INVALID_MESSAGE_ID].toUInt());
//    this->m_conversation_list_model->considerNewValidatedTextMessage(valid_message_info, invalid_message_id);
//    this->m_message_list_model->considerNewTextMessage(valid_message_info, false);
}


// private
void DataHandler::handleFetchResult(const NetInfoContainer &net_message)
{
    using namespace KeyWords;

    if (net_message.contains(PRIVATE_CHATS))
    {
        this->m_db->checkForAnyPrivateEnvUpdate(net_message[PRIVATE_CHATS].toArray());
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

    auto valid_envs = this->m_db->getAllValidEnvs();
    auto number_of_valid_envs = valid_envs->size();
    auto pendings = this->m_db->getAllPendingEnvs();
    quint16 i = 0;
    auto& new_data = this->m_conversation_list_model->m_conversations;
    new_data.clear();
    new_data.reserve(valid_envs->size() + pendings->size());

    for (; i < number_of_valid_envs; i++)
        new_data.emplace_back(
                    valid_envs->value(i)[ENV_ID].toUInt(),
                    false,
                    this->m_db->getNameOfUser(valid_envs->value(i)[OTHER_PERSON_ID].toUInt()),
                    this->m_db->getLastEnvMessageId(valid_envs->value(i)[ENV_ID].toUInt())
                    );

    for (; i < new_data.capacity(); i++)
        new_data.emplace_back(
                    pendings->value(i - number_of_valid_envs)[INVALID_ENV_ID].toUInt(),
                    true,
                    this->m_db->getNameOfUser(pendings->value(i)[OTHER_PERSON_ID].toUInt()),
                    this->m_db->getMaxMessagesId()
                    );

    this->m_conversation_list_model->endResetModel();
}


QString DataHandler::getProperConversationHeader(const quint64 &env_id,
                                                 const bool& is_pending)
{
    if(!is_pending && this->m_db->isValidPrivateChat(env_id))
        return this->m_db->getOtherPersonNameInPrivateChat(env_id, false);
    else if(is_pending && this->m_db->isPendingPrivateChat(env_id))
        return this->m_db->getOtherPersonNameInPrivateChat(env_id, true);

    qDebug() << "wasn't private!";
    return QString();
}

quint64 DataHandler::insertPendingTextMessage(const quint64 &env_id, const bool &is_env_pending, const QString &message_text)
{
    return this->m_db->insertPendingTextMessage(env_id, message_text, is_env_pending);
}

