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

signals:

public slots:
    void startSearch(QUrl url, QString text, int url_count, int thread_count = 1);
    void addTask(QUrl task);
    void stopAllThreads();
    void addPage(QString page);

private slots:
    void workRequested();

private:
    QList<QUrl> _tasks;
    QVector<QThread *> _threads;
    QVector<QHttpWorker *> _workers;
    QStringList _pages;
    int _max_task_count;
    int _current_task;
    QMutex _access_mutex;
    QSemaphore _task_sem;
    QString _text;
    //QList<int>
};

#endif // THREADCONTROLLER_H
