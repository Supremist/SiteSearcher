#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QThread>
#include "httpworker.h"
#include "qtasklist.h"

class QThreadController : public QObject
{
    Q_OBJECT
public:
    explicit QThreadController(QObject *parent = 0);
    ~QThreadController();

signals:
    void textFound(QString header, QString page, QVector<QPoint> positions);

public slots:
    void startSearch(QUrl url, QString text, int url_count, int thread_count = 1);
    void addTask(QUrl task);
    void stopAllThreads();

private slots:
    void workRequested(int worker_id);

private:
    QVector<QThread *> _threads;
    QVector<QHttpWorker *> _workers;
    QList<QUrl> _tasks;
    int _max_task_count;
    int _current_task;
    QMutex _access_mutex;
    QSemaphore _task_sem;
    QString _text;
    //QList<int>
};

#endif // THREADCONTROLLER_H
