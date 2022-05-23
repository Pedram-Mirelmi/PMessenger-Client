#include "NetPendingTaskManager.h"


NetPendingTaskManager::NetPendingTaskManager(QObject* parent)
    : QObject(parent)
{}

void NetPendingTaskManager::addUserInfoRelatedTask(const quint64 &user_id, std::function<void (QJsonObject)> task)
{
    m_user_info_tasks[user_id].append(task);
}

void NetPendingTaskManager::finishUserInfoRelatedTask(const quint64 &user_id, const QJsonObject &info)
{
    auto tasks = m_user_info_tasks.take(user_id);
    for(auto itter = tasks.cbegin(); itter != tasks.cend(); itter++)
        itter->operator()(info);
}
