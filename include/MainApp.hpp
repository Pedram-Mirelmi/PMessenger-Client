#pragma once
#include <QObject>
#include <QThread>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QJsonObject>
#include "./NetworkPackages/NetworkHandler.hpp"
#include "./DataHandlingPackages/DataHandler.hpp"

static QJsonObject user_info;

class MainApp : public QObject
{
    Q_OBJECT
public:
    NetworkHandler* m_network_handler;
    DataHandler* m_data_handler;
    explicit MainApp (QQmlApplicationEngine& qml_engine,
                      QObject* parent = nullptr,
                      const QString& address = "127.0.0.1",
                      const quint16 port = 54000);

    Q_INVOKABLE void openPrivateChatWith(const quint64 user_id,
                                         const QString& name);

    Q_INVOKABLE void openExistingChatEnv(const quint64& env_id,
                                         const bool& is_pending);

    Q_INVOKABLE void sendNewTextMessage(const quint64& env_id,
                                        const QString& message_text,
                                        const bool& to_pending);

private:
    InfoContainer m_user_info;

private slots:
    void initiateApp(const QJsonObject& nappendConversationet_msg);
};
