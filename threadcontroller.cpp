#include "threadcontroller.h"

QThreadController::QThreadController(QObject *parent) : QObject(parent)
{

}

QThreadController::~QThreadController()
{
    stopAllThreads();
}

void QThreadController::startSearch(QUrl url, QString text, int url_count, int thread_count)
{
    stopAllThreads();
    _tasks.clear();
    addTask(url);
    _max_task_count = url_count;
    _current_task = 0;
    _text = text;
    _threads.resize(thread_count);
    _workers.resize(thread_count);
    for(int i = 0; i<thread_count; ++i) {
        _threads[i] = new QThread();
        _workers[i] = new QHttpWorker();
        _workers[i]->moveToThread(_threads[i]);
        //connect(_workers[i], SIGNAL(error(QString)), this, SLOT(errorString(QString)));
        connect(_workers[i], SIGNAL(requestWork()), this, SLOT(workRequested()));
        connect(_workers[i], SIGNAL(urlFound(QUrl)), this, SLOT(addTask(QUrl)));
        connect(_threads[i], SIGNAL(started()), _workers[i], SLOT(start()));
        connect(_workers[i], SIGNAL(finished()), _threads[i], SLOT(quit()));
        connect(_workers[i], SIGNAL(finished()), _workers[i], SLOT(deleteLater()));
        connect(_threads[i], SIGNAL(finished()), _threads[i], SLOT(deleteLater()));
        _threads[i]->start();
    }
}

void QThreadController::addTask(QUrl task)
{
    QMutexLocker locker(_access_mutex);
    if (_tasks.indexOf(task) == -1){
        _tasks.push_back(task);
        _task_sem.release();
    }
}

void QThreadController::stopAllThreads()
{
    foreach (QHttpWorker * worker, _workers) {
        worker->stop();
    }
}

void QThreadController::workRequested()
{
    QMutexLocker locker(_access_mutex);
    QHttpWorker * worker = qobject_cast<QHttpWorker>(sender());
    if (worker){

    }
}

