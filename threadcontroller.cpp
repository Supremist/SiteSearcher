#include "threadcontroller.h"

QThreadController::QThreadController(QObject *parent) : QObject(parent)
{

}

QThreadController::~QThreadController()
{
    stopAllThreads();
}

int QThreadController::getFreeWorker()
{
    for (int i = 0; i<_isWorking.size(); ++i){
        if (!_isWorking[i])
            return i;
    }
    return -1;
}

void QThreadController::startSearch(QUrl url, QString text, int url_count, int thread_count)
{
    stopAllThreads();
    _tasks.clear();
    _max_task_count = url_count;
    _current_task = 0;
    _free_workers_count = thread_count;
    _text = text;
    _threads.resize(thread_count);
    _workers.resize(thread_count);
    _isWorking.resize(thread_count);
    _isWorking.fill(false);
    for(int i = 0; i<thread_count; ++i) {
        _threads[i] = new QThread();
        _workers[i] = new QHttpWorker();
        _workers[i]->setId(i);
        _workers[i]->moveToThread(_threads[i]);
        //connect(_workers[i], SIGNAL(error(QString)), this, SLOT(errorString(QString)));
        connect(_workers[i], SIGNAL(requestWork(int)), this, SLOT(workRequested(int))); //, Qt::DirectConnection);
        connect(_workers[i], SIGNAL(urlFound(QUrl)), this, SLOT(addTask(QUrl)));
        connect(this, SIGNAL(stopAll()), _workers[i], SLOT(stop()));
        connect(this, SIGNAL(stopAll()), _threads[i], SLOT(quit()));
        connect(_threads[i], SIGNAL(started()), _workers[i], SLOT(start()));
        //connect(_workers[i], SIGNAL(finished()), _threads[i], SLOT(quit()));
        connect(_workers[i], SIGNAL(finished()), _workers[i], SLOT(deleteLater()));
        connect(_threads[i], SIGNAL(finished()), _threads[i], SLOT(deleteLater()));

        connect(_workers[i], SIGNAL(searchFinished(QString,QString,QVector<QPoint>)),
                this, SIGNAL(textFound(QString,QString,QVector<QPoint>)));

        _threads[i]->start();
    }
    addTask(url);
}

void QThreadController::addTask(QUrl task)
{
    //QMutexLocker locker(&_access_mutex);
    if (_tasks.indexOf(task) == -1){
        _tasks.push_back(task);
        //_task_sem.release();
        int free_worker;
        if (_free_workers_count > 0 && (free_worker = getFreeWorker()) != -1){
            sendTask(free_worker);
        }
    }
}


void QThreadController::stopAllThreads()
{
    emit stopAll();
    for (int i = 0; i < _workers.size(); ++i){
        qDebug()<<"wait "<<i;
        //_threads[i]->quit();
        _threads[i]->wait();
    }
    _workers.clear();
    _threads.clear();
}


void QThreadController::workRequested(int worker_id)
{
    //QMutexLocker locker(&_access_mutex);
    if (worker_id < _workers.size()){
        _free_workers_count++;
        _isWorking[worker_id] = false;
        if (_current_task < _tasks.size())
            sendTask(worker_id);
        if (_free_workers_count == _workers.size()){
            stopAllThreads();
        }
    }
}

void QThreadController::sendTask(int worker_id)
{
    if (_current_task < _max_task_count){
        _free_workers_count--;
        _isWorking[worker_id] = true;
        QMetaObject::invokeMethod(_workers[worker_id], "startSearch", Qt::QueuedConnection,
                                       Q_ARG(QUrl, _tasks[_current_task++]),
                                       Q_ARG(QString, _text));
    }
}

