#include "ConversationsListModel.hpp"
#include "../Commons/KeyWords.hpp"
#include "../../ClientKeywords.hpp"
#include <QJsonObject>

ConversationsListModel::ConversationsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

quint16 ConversationsListModel::currChatIndex() const
{
    return m_current_chat_index;
}

QString ConversationsListModel::currChatTitle() const
{
    if(this->m_conversations.size())
        return this->m_conversations[this->currChatIndex()].title;
    return QString();
}

void ConversationsListModel::setCurrChatIndex(const quint16 &index)
{
    this->m_current_chat_index = index;
    this->setCurrChatTitle(this->m_conversations[index].title);
    emit this->currChatIndexChanged(index);
}

void ConversationsListModel::setCurrChatTitle(const QString &new_title)
{
    emit this->currChatTitleChanged(new_title);
}

void ConversationsListModel::selectChat(const quint64 &env_id, const bool &is_pending)
{
    auto index = this->searchFromTop(env_id, is_pending);
    this->setCurrChatIndex(index);
}


int ConversationsListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return this->m_conversations.size();
}


QVariant ConversationsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case ConversationsListModel::Roles::env_id:
        return this->m_conversations[index.row()].env_id;
    case ConversationsListModel::Roles::title:
        return this->m_conversations[index.row()].title;
    case ConversationsListModel::Roles::is_pending:
        return this->m_conversations[index.row()].is_pending;
    case ConversationsListModel::Roles::last_message_id:
        return this->m_conversations[index.row()].last_message_id;
    }

    return QVariant();
}

bool ConversationsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        switch (role)
        {
        case ConversationsListModel::Roles::is_pending:
            this->m_conversations[index.row()].is_pending = value.toBool();
            break;
        case ConversationsListModel::Roles::title:
            this->m_conversations[index.row()].title = value.toString();
            break;
        case ConversationsListModel::Roles::env_id:
            this->m_conversations[index.row()].env_id = value.toUInt();
            break;
        case ConversationsListModel::Roles::last_message_id:
            this->m_conversations[index.row()].last_message_id = value.toUInt();
            break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ConversationsListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> ConversationsListModel::roleNames() const
{
    using namespace KeyWords;
    QHash<int, QByteArray> names;
    names[ConversationsListModel::Roles::env_id] = ENV_ID;
    names[ConversationsListModel::Roles::is_pending] = IS_PENDING;
    names[ConversationsListModel::Roles::title] = TITLE;
    names[ConversationsListModel::Roles::last_message_id] = LAST_MESSAGE_ID;
    return names;
}

void ConversationsListModel::sortConversatoins() // insertion sort
{
    using namespace KeyWords;
    for (auto i = 0; i < this->m_conversations.size() - 1; i++)
    {
        auto j = i + 1;
        while (j >= 1 && this->m_conversations[j - 1].last_message_id >
               this->m_conversations[j].last_message_id)
        {
            swapItems(j, j - 1);
            j--;
        }
    }
}

void ConversationsListModel::appendConversation(const ConversatonItem &conversation)
{
    this->beginInsertRows(QModelIndex(),
                          this->m_conversations.size(),
                          this->m_conversations.size());
    this->m_conversations.append(conversation);
    this->endInsertRows();
}

void ConversationsListModel::tryToInsertConversation(const ConversatonItem &conversation)
{
    if(searchFromTop(conversation.env_id, conversation.is_pending)
            == this->m_conversations.size())
    {
        this->appendConversation(conversation);
        auto index = this->m_conversations.size() - 2;
        while (this->m_conversations[index].last_message_id >
               conversation.last_message_id)
        {
            swapItems(index, index+1);
            index--;
        }
    }
}

void ConversationsListModel::changeConversationToValid(const quint64 &invalid_id)
{
    auto index = this->searchFromTop(invalid_id, true);
    this->m_conversations[index].is_pending = false;
}

void ConversationsListModel::popUpConversation(const QJsonObject& new_inserted_msg)
{
//    using namespace KeyWords;
//    auto conv_index_in_model = this->searchFromTop(new_inserted_msg[ENV_ID].toInteger(), true);
//    if ((quint64)new_inserted_msg[MESSAGE_ID].toInteger() >
//            this->m_conversations[conv_index_in_model].last_message_id)
//    {
//        while (conv_index_in_model < this->m_conversations.size() - 1 &&
//               this->m_conversations[conv_index_in_model].last_message_id >
//               this->m_conversations[conv_index_in_model].last_message_id)
//        {
//            this->swapItems(conv_index_in_model, conv_index_in_model + 1);
//            conv_index_in_model++;
//        }
//    }
}

void ConversationsListModel::swapItems(const quint64 &first, const quint64 &second)
{
    auto temp = this->m_conversations[first];
    this->m_conversations[first] = this->m_conversations[second];
    this->m_conversations[second] = temp;
    emit this->dataChanged(this->index(first, 0), this->index(first, 0),
                           QVector<int>() << Roles::last_message_id << Roles::title);
    emit this->dataChanged(this->index(second, 0), this->index(second, 0),
                           QVector<int>() << Roles::last_message_id << Roles::title);
}

quint16 ConversationsListModel::searchFromTop(const quint64 &env_id,
                                              const bool &is_pending)
{
    qint32 index = this->m_conversations.size() - 1;
    while(index >= 0 &&
          this->m_conversations[index].env_id != env_id &&
          this->m_conversations[index].is_pending != is_pending)
    {
        index--;
    }
    return index == -1 ? this->m_conversations.size() : index;
}
