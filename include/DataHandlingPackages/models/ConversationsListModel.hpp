#pragma once
#include <QAbstractItemModel>
#include "../../Others/TypeDefs.hpp"

class DataHandler;

struct ConversatonItem
{
    ConversatonItem(const quint64& env, const bool& pending,
                    const QString& titlee, const quint64& msg_id)
        : env_id(env), is_pending(pending),
          title(titlee), last_message_id(msg_id)
    {};
    quint64 env_id;
    bool is_pending;
    QString title;
    quint64 last_message_id;
    bool operator==(const ConversatonItem& other) const;
};


class ConversationsListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(quint16 currChatIndex READ currChatIndex WRITE setCurrChatIndex NOTIFY currChatIndexChanged)
    Q_PROPERTY(QString currChatTitle READ currChatTitle WRITE setCurrChatTitle NOTIFY currChatTitleChanged)
    quint16 m_current_chat_index = 0;

    QVector<ConversatonItem> m_conversations;

    enum Roles
    {
        env_id = Qt::UserRole,
        is_pending,
        title,
        last_message_id
    };

    friend class DataHandler;

public:
    explicit ConversationsListModel(QObject *parent = nullptr);

    quint16 currChatIndex() const;

    QString currChatTitle() const;

    void setCurrChatIndex(const quint16& index);

    void setCurrChatTitle(const QString& new_title);

    void selectChat(const quint64& env_id,
                    const bool& is_pending);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void considerNewTextMessage(const InfoContainer &valid_message_info);

    void insertEnv(const ConversatonItem& conversation);

    void sortConversations(); // insertion sort

    void appendConversation(const ConversatonItem& conversation);

    void tryToInsertConversation(const ConversatonItem& conversation);

    void changeConversationToValid(const quint64& invalid_id,
                                   const quint64& new_valid_env_id);

public slots:
    void popUpConversation(const QJsonObject& new_inserted_msg);
    void considerValidatedPrivateEnv(const quint64& invalid_env_id,
                               const quint64& valid_env_id);
private:

    void swapItems(const quint64 &first, const quint64 &second);

    quint16 searchFromTop(const quint64& env_id,
                          const bool& is_pending);
signals:
    void currChatIndexChanged(const quint16& index);
    void currChatTitleChanged(const QString& new_title);
};
