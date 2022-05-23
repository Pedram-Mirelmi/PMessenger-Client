#pragma once
/*
 *  A class containing and managing all the tasks that are waiting
 *  for a network response
 *
 *  class DataHandler holds this class
 *
 *  example:
 *      after a making a FETCH request, in the chat envs list we see a new chat env id that
 *      we don't have in our database. we have a new task! to add this new chat to UI but we need more details.
 *      so we send a request to get more details about this chat env.
 *      The problem is here that the RECEIVER of the information is
 *      working completly independent and has no idea we needed that info earlier and
 *      where and how to use it
 *
 *
 *  So this class corresponds a lambda function that does the task when it has the info
 *  Also each time that DataHandler receives a new DATA from network it pass it here so
 *  NetPendingTaskManager can check and finish any task needing that info.
 *
 */

#include <QObject>
#include <QJsonObject>
#include <QHash>

class NetPendingTaskManager : QObject
{
    typedef quint64 IdType;
    QHash<IdType, QVector<std::function<void(QJsonObject)>>> m_user_info_tasks;
public:
    explicit NetPendingTaskManager(QObject* parent = nullptr);

    // user info tasks
    void addUserInfoRelatedTask(const quint64& user_id, std::function<void(QJsonObject)> task);

    void finishUserInfoRelatedTask(const quint64& user_id, const QJsonObject& info);
};
