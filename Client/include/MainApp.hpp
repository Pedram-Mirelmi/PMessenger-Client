#pragma once
#include <QObject>
#include <thread>
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

    Q_INVOKABLE void test()
    {
        this->m_data_handler->m_conversation_list_model->tryToAppendConversation({
                                                                                     1,
                                                                                     true,
                                                                                     "Mina",
                                                                                     1
                                                                                 });
    }
    Q_INVOKABLE void test2()
    {
        using namespace KeyWords;
        InfoContainer info;
        info[MESSAGE_TEXT] = "text text text text text text text ";
        info[MESSAGE_ID] = 1;
        this->m_data_handler->m_message_list_model->insertMessage(info);
    }
private:
    InfoContainer m_user_info;

private slots:
    void initiateApp(const QJsonObject& net_msg);
};
