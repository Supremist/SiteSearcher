#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QList>
#include <QVector>
#include <QThread>
#include "httpworker.h"

class QThreadController : public QObject
{
    Q_OBJECT
public:
    explicit QThreadController(QObject *parent = 0);
    ~QThreadController();
    int getFreeWorker();

signals:
    void textFound(QString header, QString page, QVector<QPoint> positions);
    void stopAll();
    void pauseAll();
    void resumeAll();
    void progressChanged(int value);
    void searchFinished();

public slots:
    void startSearch(QUrl url, QString text, int url_count, int thread_count = 1, bool search_all = true, bool case_sens = true);
    void addTask(QUrl task);
    void stopAllThreads();
    void pauseAllThreads();
    void resumeAllThreads();

private slots:
    void workRequested(int worker_id);
    void pageLoaded(QString header, QString page, QVector<QPoint> positions);

private:
    void sendTask(int worker_id);

    QVector<QThread *> _threads;
    QVector<QHttpWorker *> _workers;
    QVector<bool> _isWorking;
    QList<QUrl> _tasks;
    int _max_task_count;
    int _current_task;
    int _finished_task;
    int _free_workers_count;
    QMutex _access_mutex;
    QSemaphore _task_sem;
    QString _text;
    bool _search_all;
    bool _case_sens;
};

#endif // THREADCONTROLLER_H
