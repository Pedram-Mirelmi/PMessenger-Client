#include "DataHandler.hpp"
#include <QMutex>
#include <QDataStream>
#include <QHash>
#include <QFile>
#include "../ClientKeywords.hpp"

static const auto pending_messages_file_path = "/home/pedram/Desktop/PMessenger/Client/data/pending_messages";
QMutex pending_messages_lock;

static const auto pending_chats_file_path = "/home/pedram/Desktop/PMessenger/Client/data/pending_chats";
QMutex pending_chats_lock;

static const auto invalid_ids_file_path = "/home/pedram/Desktop/PMessenger/Client/data/invalid_ids";
QMutex invalid_ids_lock;

QHash<quint64, InfoContainer> pending_messages,
                              pending_chats;

InfoContainer invalid_ids;

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

void DataHandler::startNewPrivateChat(const quint64 &user_id)
{
    this->addPrivateChatToPendingChats(user_id);
    this->m_net_handler->sendCreateNewPrivateChatReq(user_id);
}

void DataHandler::sendNewTextMessage(const quint64 &env_id, const QString &message_text)
{
    this->addNewTextMessageToPendingMessages(env_id, message_text);
    this->m_net_handler->sendNewTextMessageReq(env_id, message_text);
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
void DataHandler::sendReqForPrivateEnvDetails(const quint64 &env_id)
{
    using namespace KeyWords;
    QJsonObject req;
    req[NET_MESSAGE_TYPE] = GET_PRIVATE_ENV_DETAILS;
    req [ENV_ID] = (quint16)env_id;
    this->m_net_handler->m_sender->sendNetMessage(req);
}

//private
void DataHandler::prepareDB()
{
    this->m_db->tryToInit();
    this->m_net_handler->sendFetchReq();
    this->tryToCreatePendingFiles();
    this->readPendingsFromFile();
}

bool DataHandler::tryToCreatePendingFiles()
{
    QFile file;
    bool inited = false;
    if (file.exists(pending_messages_file_path))
    {
        QFile pending_messages_file(pending_messages_file_path);
        pending_messages_file.open(QFile::ReadWrite);
        pending_messages_file.close();
        serializeToFile(pending_messages, pending_messages_file_path);
        inited = true;
    }
    if (file.exists(pending_chats_file_path))
    {
        QFile pending_chats_file (pending_chats_file_path);
        pending_chats_file.open(QFile::ReadWrite);
        pending_chats_file.close();
        serializeToFile(pending_chats, pending_chats_file_path);
        inited = true;
    }
    if (!file.exists(invalid_ids_file_path))
    {
        using namespace KeyWords;
        QFile invalid_ids_file(invalid_ids_file_path);
        invalid_ids_file.open(QFile::ReadWrite);
        invalid_ids_file.close();
        invalid_ids[LAST_INVALID_CHAT_ID] = invalid_ids[LAST_INVALID_MESSAGE_ID] = 0;
        serializeToFile(invalid_ids, invalid_ids_file_path);
        inited = true;
    }
    return inited;

}

// private
void DataHandler::readPendingsFromFile() // thread safe
{
    pending_messages_lock.lock();
    this->deserializeFromFile(pending_messages, pending_messages_file_path);
    pending_messages_lock.unlock();

    pending_chats_lock.lock();
    this->deserializeFromFile(pending_chats, pending_chats_file_path);
    pending_chats_lock.unlock();

    invalid_ids_lock.lock();
    this->deserializeFromFile(invalid_ids, invalid_ids_file_path);
    invalid_ids_lock.unlock();
}

//private
void DataHandler::addPrivateChatToPendingChats(const quint64 &user_id)
{
    pending_chats_lock.lock();

    using namespace KeyWords;
    InfoContainer chat_info;
    chat_info[FIRST_PERSON] = this->this_user_id;
    chat_info[SECOND_PERSON] = user_id;
    chat_info[ENV_ID] = invalid_ids[LAST_INVALID_CHAT_ID] = invalid_ids[LAST_INVALID_CHAT_ID].toUInt() + 1;

    pending_chats_lock.unlock();
}

void DataHandler::addNewTextMessageToPendingMessages(const quint64 &env_id, const QString &message_text)
{
    pending_messages_lock.lock();

    using namespace KeyWords;
    InfoContainer message_info;
    message_info[MESSAGE_TEXT] = message_text;
    message_info[OWNER_ID] = this->this_user_id;
    message_info[ENV_ID] = env_id;
    message_info[MESSAGE_ID] = invalid_ids[LAST_INVALID_MESSAGE_ID] = invalid_ids[LAST_INVALID_MESSAGE_ID].toUInt() + 1;

    pending_messages_lock.unlock();
}


template<typename Loader_T>
bool DataHandler::deserializeFromFile(Loader_T &data_to_deserialize, const char *file_path)
{
    QFile file(file_path);
    if (!file.open(QFile::ReadOnly))
    {
        qCritical() << "couldn't open file " << file_path;
        return false;
    }
    QDataStream stream(&file);
    if (stream.version() != QDataStream::Qt_6_2)
        qCritical() << "INVALID Qt version!";

    stream >> data_to_deserialize;
    file.close();
    return true;
}

template<typename Loader_T>
bool DataHandler::serializeToFile(Loader_T &data_to_serialize, const char *file_path)
{
    QFile file(file_path);
    if (!file.open(QFile::WriteOnly))
    {
        qCritical() << "couldn't open file " << file_path;
        return false;
    }
    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_2);

    stream << data_to_serialize;
    file.close();
    return true;
}

// private
void DataHandler::convertToHash(InfoContainer &target, const QJsonObject &source)
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}


