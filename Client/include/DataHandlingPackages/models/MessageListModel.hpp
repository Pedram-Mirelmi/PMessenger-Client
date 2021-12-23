#pragma once
#include <QAbstractListModel>
#include <QJsonObject>
#include <QVector>
#include <QList>
#include "./ItemsStructures.hpp"


class DataHandler;


class MessageListModel : public QAbstractListModel
{
    Q_OBJECT

    friend class DataHandler;
public:
    explicit MessageListModel(QObject *parent = nullptr);

    enum Roles
    {
        message_id = Qt::UserRole,
        owner_id,
//        created_at,
        message_type
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QHash<int, QByteArray> roleNames() const override;

    void insertMessage(const QHash<const char*, QVariant> & msg);
    void insertMessages(QVector<QHash<const char*, QVariant>> &msg_list);
private:
    QVector<QHash<const char* , QVariant>> m_messages;

    void clearModel();

    void swapItems(const quint64 &first, const quint64 &second);
    void sortMessages();
};

