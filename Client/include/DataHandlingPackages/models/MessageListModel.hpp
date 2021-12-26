#pragma once
#include <QAbstractListModel>
#include <QJsonObject>
#include <QVector>
#include <QList>
#include "../../Others/TypeDefs.hpp"


class DataHandler;

class MessageListModel : public QAbstractListModel
{
    Q_OBJECT
    quint64 current_env_id;
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

    void insertMessage(const InfoContainer& msg);
    void insertMessages(QVector<InfoContainer> &msg_list);

public slots:
    void considerNewTextMessage(const QJsonObject& msg_info);
private:
    QVector<InfoContainer> m_messages;

    void clearModel();

    void swapItems(const quint64 &first, const quint64 &second);
    void sortMessages();

    void convertToHash(InfoContainer& target, const QJsonObject& source); // in place
};

