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
    Q_PROPERTY(quint64 curr_env_id READ curr_env_id WRITE setCurr_env_id NOTIFY curr_env_idChanged)
    quint64 current_env_id;
    friend class DataHandler;
public:
    quint64 curr_env_id();
    quint64 setCurr_env_id(const quint64& env_id);
signals:
    void curr_env_idChanged(const quint64& env_id);
public:
    explicit MessageListModel(QObject *parent = nullptr);

    enum Roles
    {
        message_id = Qt::UserRole,
        env_id,
        owner_id,
        message_type,
        message_text,
        is_pending
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
    void considerNewTextMessage(const InfoContainer& msg_info);

private:
    std::shared_ptr<InfoCollection> m_messages;

    void clearModel();

    void swapItems(const quint64 &first, const quint64 &second);

    void sortMessages();

    InfoContainer convertToHash(const NetInfoContainer &source);
};

