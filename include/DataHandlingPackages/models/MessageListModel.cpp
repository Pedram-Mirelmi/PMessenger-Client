#include "MessageListModel.hpp"
#include "../../../KeyWords.hpp"
#include "../../ClientKeywords.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>




quint64 MessageListModel::curr_env_id()
{
    return this->current_env_id;
}

quint64 MessageListModel::setCurr_env_id(const quint64 &env_id)
{
    this->current_env_id = env_id;
    emit this->curr_env_idChanged(this->current_env_id);
}

MessageListModel::MessageListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_messages(std::make_shared<InfoCollection>())
{
    using namespace KeyWords;
//    InfoContainer msg1, msg2;
//    msg1[MESSAGE_TEXT] = "Hiiiii";
//    msg2[MESSAGE_TEXT] = "heeeey";

//    this->m_messages->append(msg1);
//    m_messages->append(msg2);
}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return this->m_messages->size();
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    using namespace KeyWords;
    switch (role)
    {
    case message_id:
        return this->m_messages->at(index.row())[MESSAGE_ID];
    case owner_id:
        return this->m_messages->at(index.row())[OWNER_ID];
    case message_text:
        return this->m_messages->at(index.row())[MESSAGE_TEXT];
//    case created_at:
//        return this->m_messages[row].created_at;
    case message_type:
        return this->m_messages->at(index.row())[MESSAGE_TYPE];
    }
    return QVariant();
}

bool MessageListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    using namespace KeyWords;
    if (data(index, role) != value)
    {
        switch (role)
        {
        case message_id:
            this->m_messages->operator[](index.row())[MESSAGE_ID] = value.toUInt();
            break;
        case owner_id:
            this->m_messages->operator[](index.row())[OWNER_ID] = value.toUInt();
            break;
//        case created_at:
//            this->m_messages[row].created_at = value.toDateTime();
//            break;
//        case message_type:
//            this->m_messages[index.row()][MESSAGE_TYPE] = value.toString();
//            break;
        }
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags MessageListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> MessageListModel::roleNames() const
{
    using namespace KeyWords;
    QHash<int, QByteArray> names;
    names[Roles::message_id] = MESSAGE_ID;
    names[Roles::env_id] = ENV_ID;
    names[Roles::owner_id] = OWNER_ID;
    names[Roles::message_text] = MESSAGE_TEXT;
    names[Roles::message_type] = MESSAGE_TYPE;
    names[Roles::is_pending] = IS_PENDING;
    return names;
};


void MessageListModel::insertMessage(const InfoContainer& msg)
{
    using namespace KeyWords;
    for (quint64 i = 0; i < (quint64)this->m_messages->size(); i++)
        if (this->m_messages->at(i)[MESSAGE_ID].toUInt() > msg[MESSAGE_ID].toUInt())
        {
            this->beginInsertRows(QModelIndex(), i, i);
            this->m_messages->insert(i, msg);
            this->endInsertRows();
            return;
        }
    this->beginInsertRows(QModelIndex(), this->m_messages->size(), this->m_messages->size());
    this->m_messages->append(msg);
    this->endInsertRows();
}


void MessageListModel::insertMessages(QVector<InfoContainer> &msg_list)
{
    using namespace KeyWords;
    for (const auto& msg_info : msg_list)
        this->insertMessage(msg_info);
}

void MessageListModel::considerNewTextMessage(const InfoContainer &msg_info)
{
    using namespace KeyWords;
    if (msg_info.contains(IS_PENDING) && msg_info[IS_PENDING].toBool())
        this->insertMessage(msg_info);
    else if ((quint64)msg_info[ENV_ID].toUInt() == this->current_env_id)
    {
        qint32 msg_index = this->m_messages->lastIndexOf(msg_info[ENV_ID].toUInt());
        if(msg_index == -1)
        {
            this->insertMessage(msg_info);
            return;
        }
        (*m_messages)[msg_index][IS_PENDING] = true;
        emit this->dataChanged(this->index(msg_index), this->index(msg_index), QVector<int>() << Roles::is_pending);
    }

}

void MessageListModel::clearModel()
{
    this->beginResetModel();
    this->m_messages->clear();
    this->endResetModel();
}

void MessageListModel::sortMessages() // insertion sort
{
    using namespace KeyWords;
    for (auto i = 0; i < this->m_messages->size() - 1; i++)
    {
        auto j = i + 1;
        while (j >= 1 && this->m_messages->at(j - 1)[MESSAGE_ID].toUInt() > this->m_messages->at(j)[MESSAGE_ID].toUInt())
        {
            swapItems(j, j - 1);
            j--;
        }
    }
}

InfoContainer MessageListModel::convertToHash(const QJsonObject &source)
{
    InfoContainer target;
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
    return target;
}

void MessageListModel::swapItems(const quint64 &first, const quint64 &second)
{
    this->m_messages->swapItemsAt(first, second);
    emit this->dataChanged(this->index(first), this->index(first),
                           QVector<int>() << Roles::message_id << Roles::owner_id << Roles::message_type);
    emit this->dataChanged(this->index(second), this->index(second),
                           QVector<int>() << Roles::message_id << Roles::owner_id << Roles::message_type);
}



