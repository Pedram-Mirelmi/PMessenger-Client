#pragma once

#include <QAbstractItemModel>
#include "../../Others/TypeDefs.hpp"

class DataHandler;

struct ConversatonItem
{
    ConversatonItem(const quint64& env, const bool& pending,
                    const QString& name, const quint64& msg_id)
        : env_id(env), is_pending(pending),
          contact_name(name), last_message_id(msg_id)
    {};
    quint64 env_id;
    bool is_pending;
    QString contact_name;
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
        env_id = Qt::UserRole,
        name,
        is_pending,
        last_message_id
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void sortConversatoins(); // insertion sort

    void tryToAppendConversation(const ConversatonItem& conversation);

    void tryToInsertConversation(const ConversatonItem& conversation);

    void changeConversationToValid(const quint64& invalid_id);

private slots:
    void popUpConversation(const QJsonObject& new_inserted_msg);
private:
    QVector<ConversatonItem> m_conversations;
    void swapItems(const quint64 &first, const quint64 &second);

    quint16 getConversationIndex(const quint32& l,
                                 const quint32& r,
                                 const quint64& env_id);

    quint16 searchFromTop(const quint64& env_id,
                          const bool& is_pending);
};
