#pragma once

#include <QAbstractItemModel>
#include "./ItemsStructures.hpp"

class ConversationsListModel : public QAbstractListModel
{
    Q_OBJECT

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

private:
    QVector<ConversatonItem> m_conversations;
    void swapItems(const quint64 &first, const quint64 &second);
    void popUpConversation(const quint32& index_in_model);
};
