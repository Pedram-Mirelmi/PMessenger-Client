#pragma once

#include <QAbstractItemModel>
#include "../../Others/TypeDefs.hpp"

class DataHandler;
struct ConversatonItem
{
    quint64 env_id;
    QString name;
    quint64 last_message_id;
};


class ConversationsListModel : public QAbstractListModel
{
    Q_OBJECT
    friend class DataHandler;
public:
    explicit ConversationsListModel(QObject *parent = nullptr);

    enum Roles
    {
        name = Qt::UserRole,
//        photo_id,
        last_message_id
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QHash<int, QByteArray> roleNames() const override;

    void sortConversatoins(); // insertion sort

private slots:
    void popUpConversation(const QJsonObject& new_inserted_msg);
private:
    QVector<ConversatonItem> m_conversations;
    void swapItems(const quint64 &first, const quint64 &second);

    quint32 getConversation(const quint32& l, const quint32& r, const quint64& env_id);
};
