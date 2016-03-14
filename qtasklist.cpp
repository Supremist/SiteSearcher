#include "qtasklist.h"

QTaskList::QTaskList(QObject *parent) : QObject(parent)
{
    clear();
}

void QTaskList::clear()
{
    _tasks.clear();
    _max_task_count = -1;
    _current_task = 0;
}

void QTaskList::addTask(QUrl task)
{
    QMutexLocker locker(&_access_mutex);
    if (_tasks.indexOf(task) == -1){
        _tasks.push_back(task);
        _task_sem.release();
    }
}

QUrl QTaskList::getTask()
{
    QMutexLocker locker(&_access_mutex);
    if (_current_task < _max_task_count){
        _task_sem.acquire();
        return _tasks[_current_task++];
    }
    else{
        emit maxTaskAchived();
        return QUrl();
    }
}
