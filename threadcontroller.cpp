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
    _max_task_count = url_count;
    _current_task = 0;
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

void QThreadController::addTask(QUrl task)
{
    QMutexLocker locker(&_access_mutex);
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
    _workers.clear();
    _threads.clear();
}


void QThreadController::workRequested(int worker_id)
{
    QMutexLocker locker(&_access_mutex);
    if (worker_id < _workers.size()){
        if (_current_task < _max_task_count){
            _task_sem.acquire();
            return _tasks[_current_task++];
        }
        else{
            emit maxTaskAchived();
            return QUrl();
        }
    }
}

