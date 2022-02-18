#include "MessageListModel.hpp"
#include "../../Commons/KeyWords.hpp"
#include "../../ClientKeywords.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QMutex>



static QMutex insert_lock;

MessageListModel::MessageListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int MessageListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return this->m_messages.size();
}

QVariant MessageListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    using namespace KeyWords;
    switch (role)
    {
    case message_id:
        return this->m_messages[index.row()][MESSAGE_ID];
    case owner_id:
        return this->m_messages[index.row()][OWNER_ID];
    case message_text:
        return this->m_messages[index.row()][MESSAGE_TEXT];
//    case created_at:
//        return this->m_messages[row].created_at;
    case message_type:
        return this->m_messages[index.row()][MESSAGE_TYPE];
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
            this->m_messages[index.row()][MESSAGE_ID] = value.toUInt();
            break;
        case owner_id:
            this->m_messages[index.row()][OWNER_ID] = value.toUInt();
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
    names[Roles::owner_id] = OWNER_ID;
//    names[Roles::message_type] = MESSAGE_TYPE;
    return names;
};


void MessageListModel::insertMessage(const InfoContainer& msg)
{
    using namespace KeyWords;
    insert_lock.lock();
    for (quint64 i = 0; i < (quint64)this->m_messages.size(); i++)
        if (this->m_messages[i][MESSAGE_ID].toUInt() > msg[MESSAGE_ID].toUInt())
        {
            this->beginInsertRows(QModelIndex(), i, i);
            this->m_messages.insert(i, msg);
            this->endInsertRows();
            return;
        }
    this->beginInsertRows(QModelIndex(), this->m_messages.size(), this->m_messages.size());
    this->m_messages.append(msg);
    this->endInsertRows();
    insert_lock.unlock();
}


void MessageListModel::insertMessages(QVector<InfoContainer> &msg_list)
{
    using namespace KeyWords;
    for (const auto& msg_info : msg_list)
        this->insertMessage(msg_info);
}

void MessageListModel::considerNewTextMessage(const QJsonObject &msg_info)
{
    using namespace KeyWords;
    if ((quint64)msg_info[ENV_ID].toInteger() == this->current_env_id)
    {
        InfoContainer model_std_container;
        this->convertToHash(model_std_container, msg_info);
        this->insertMessage(model_std_container);
    }
}

void MessageListModel::clearModel()
{

}

void MessageListModel::sortMessages() // insertion sort
{
    using namespace KeyWords;
    for (auto i = 0; i < this->m_messages.size() - 1; i++)
    {
        auto j = i + 1;
        while (j >= 1 && this->m_messages[j - 1][MESSAGE_ID].toUInt() > this->m_messages[j][MESSAGE_ID].toUInt())
        {
            swapItems(j, j - 1);
            j--;
        }
    }
}

void MessageListModel::convertToHash(InfoContainer &target, const QJsonObject &source) // in place
{
    for (const auto& key : source.keys())
        target[key.toStdString().c_str()] = source[key].toVariant();
}

void MessageListModel::swapItems(const quint64 &first, const quint64 &second)
{
    auto temp = this->m_messages[first];
    this->m_messages[first] = this->m_messages[second];
    this->m_messages[second] = temp;
    emit this->dataChanged(this->index(first, 0), this->index(first, 0),
                           QVector<int>() << Roles::message_id << Roles::owner_id << Roles::message_type);
    emit this->dataChanged(this->index(second, 0), this->index(second, 0),
                           QVector<int>() << Roles::message_id << Roles::owner_id << Roles::message_type);
}



