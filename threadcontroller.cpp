#include "threadcontroller.h"

QThreadController::QThreadController(QObject *parent) : QObject(parent)
{
    _tasks = new QTaskList();
}

QThreadController::~QThreadController()
{
    stopAllThreads();
}

void QThreadController::startSearch(QUrl url, QString text, int url_count, int thread_count)
{
    stopAllThreads();
    _tasks->clear();
    _tasks->setMaxTaskCount(url_count);
    _tasks->addTask(url);

    _text = text;
    _threads.resize(thread_count);
    _workers.resize(thread_count);
    for(int i = 0; i<thread_count; ++i) {
        _threads[i] = new QThread();
        _workers[i] = new QHttpWorker();
        _workers[i]->moveToThread(_threads[i]);
        //connect(_workers[i], SIGNAL(error(QString)), this, SLOT(errorString(QString)));
        connect(_workers[i], SIGNAL(requestWork()), this, SLOT(workRequested()), Qt::DirectConnection);
        connect(_workers[i], SIGNAL(urlFound(QUrl)), this, SLOT(addTask(QUrl)), Qt::DirectConnection);
        connect(_threads[i], SIGNAL(started()), _workers[i], SLOT(start()));
        connect(_workers[i], SIGNAL(finished()), _threads[i], SLOT(quit()));
        connect(_workers[i], SIGNAL(finished()), _workers[i], SLOT(deleteLater()));
        connect(_threads[i], SIGNAL(finished()), _threads[i], SLOT(deleteLater()));

        connect(_workers[i], SIGNAL(searchFinished(QString,QString,QVector<QPoint>)),
                this, SIGNAL(textFound(QString,QString,QVector<QPoint>)));

        _threads[i]->start();
    }
}


void QThreadController::stopAllThreads()
{
    foreach (QHttpWorker * worker, _workers) {
        worker->stop();
    }
    _workers.clear();
    _threads.clear();
}


void QThreadController::workRequested()
{
    QMutexLocker locker(&_access_mutex);
    QHttpWorker * worker = qobject_cast<QHttpWorker *>(sender());
    if (worker){

    }
}

