#ifndef QTASKLIST_H
#define QTASKLIST_H

#include <QObject>
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>
#include <QList>
#include <QUrl>

class QTaskList : public QObject
{
    Q_OBJECT
public:
    explicit QTaskList(QObject *parent = 0);
    void setMaxTaskCount(int max_task_count){_max_task_count = max_task_count;}
    void clear();
    void addTask(QUrl task);
    QUrl getTask();

signals:
    void maxTaskAchived();

public slots:

private:
    QList<QUrl> _tasks;
    int _max_task_count;
    int _current_task;
    QMutex _access_mutex;
    QSemaphore _task_sem;
};

#endif // QTASKLIST_H
