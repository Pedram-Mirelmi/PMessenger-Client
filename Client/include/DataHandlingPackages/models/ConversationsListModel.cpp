#include "ConversationsListModel.hpp"
#include "../Commons/KeyWords.hpp"
#include "../../ClientKeywords.hpp"
#include <QJsonObject>

ConversationsListModel::ConversationsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int ConversationsListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return this->m_conversations.size();
}


QVariant ConversationsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case ConversationsListModel::Roles::name:
        return this->m_conversations[index.row()].name;
//    case ConversationsListModel::Roles::photo_id:
//        return this->m_conversations[index.row()].photo_id;
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
        case ConversationsListModel::Roles::name:
            this->m_conversations[index.row()].name = value.toString();
            break;
    //    case ConversationsListModel::Roles::photo_id:
    //        this->m_conversations[index.row()].photo_id = value.toUInt();
//            break
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
    names[ConversationsListModel::Roles::name] = NAME;
    names[ConversationsListModel::Roles::last_message_id] = LAST_MESSAGE_ID;
//    names[ConversationsListModel::Roles::photo_id] = PHOTO_ID;
    return names;
}

void ConversationsListModel::sortConversatoins() // insertion sort
{
    using namespace KeyWords;
    for (auto i = 0; i < this->m_conversations.size() - 1; i++)
    {
        auto j = i + 1;
        while (j >= 1 && this->m_conversations[j - 1].last_message_id > this->m_conversations[j].last_message_id)
        {
            swapItems(j, j - 1);
            j--;
        }
    }
}

void ConversationsListModel::popUpConversation(const QJsonObject& new_inserted_msg)
{
    using namespace KeyWords;
    auto conv_index_in_model = this->getConversation(0, this->m_conversations.size(),
                                                            new_inserted_msg[ENV_ID].toInteger());
    if ((quint64)new_inserted_msg[MESSAGE_ID].toInteger() > this->m_conversations[conv_index_in_model].last_message_id)
        while (conv_index_in_model < this->m_conversations.size() - 1 &&
               this->m_conversations[conv_index_in_model].last_message_id > this->m_conversations[conv_index_in_model].last_message_id)
        {
            this->swapItems(conv_index_in_model, conv_index_in_model + 1);
            conv_index_in_model++;
        }
}

void ConversationsListModel::swapItems(const quint64 &first, const quint64 &second)
{
    auto temp = this->m_conversations[first];
    this->m_conversations[first] = this->m_conversations[second];
    this->m_conversations[second] = temp;
    emit this->dataChanged(this->index(first, 0), this->index(first, 0), QVector<int>() << Roles::last_message_id << Roles::name);
    emit this->dataChanged(this->index(second, 0), this->index(second, 0), QVector<int>() << Roles::last_message_id << Roles::name);
}

quint32 ConversationsListModel::getConversation(const quint32& l, const quint32& r, const quint64& env_id)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;
        if (this->m_conversations[mid].env_id == env_id)
            return mid;
        if (this->m_conversations[mid].env_id > env_id)
            return this->getConversation(l, mid - 1, env_id);
        return getConversation(mid + 1, r, env_id);
    }
    return -1;
}
