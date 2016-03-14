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

void QThreadController::startSearch(QUrl url, QString text, int url_count, int thread_count, bool search_all, bool case_sens)
{
    stopAllThreads();
    _search_all = search_all;
    _case_sens = case_sens;
    _tasks.clear();
    _max_task_count = url_count;
    _current_task = 0;
    _finished_task = 0;
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
        _workers[i]->setCaseSensitive(case_sens);
        _workers[i]->moveToThread(_threads[i]);

        connect(_workers[i], SIGNAL(requestWork(int)), this, SLOT(workRequested(int)));
        connect(_workers[i], SIGNAL(urlFound(QUrl)), this, SLOT(addTask(QUrl)));

        connect(this, SIGNAL(stopAll()), _workers[i], SLOT(stop()));
        connect(this, SIGNAL(stopAll()), _threads[i], SLOT(quit()));

        connect(this, SIGNAL(pauseAll()), _workers[i], SLOT(pause()), Qt::DirectConnection);
        connect(this, SIGNAL(resumeAll()), _workers[i], SLOT(resume()), Qt::DirectConnection);

        connect(_threads[i], SIGNAL(started()), _workers[i], SLOT(start()));

        //connect(_workers[i], SIGNAL(finished()), _threads[i], SLOT(quit()));
        connect(_workers[i], SIGNAL(finished()), _workers[i], SLOT(deleteLater()));
        connect(_threads[i], SIGNAL(finished()), _threads[i], SLOT(deleteLater()));

        connect(_workers[i], SIGNAL(searchFinished(QString,QString,QVector<QPoint>)),
                this, SLOT(pageLoaded(QString,QString,QVector<QPoint>)));

        _threads[i]->start();
    }
    addTask(url);
}

void QThreadController::addTask(QUrl task)
{
    if (_tasks.indexOf(task) == -1){
        _tasks.push_back(task);
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
        _threads[i]->wait();
    }
    _workers.clear();
    _threads.clear();
    emit searchFinished();
    emit progressChanged(100);
}

void QThreadController::pauseAllThreads()
{
    emit pauseAll();
}

void QThreadController::resumeAllThreads()
{
    emit resumeAll();
}


void QThreadController::workRequested(int worker_id)
{
    if (worker_id < _workers.size()){
        _finished_task++;
        _free_workers_count++;
        _isWorking[worker_id] = false;
        emit progressChanged(round(_finished_task*100.0/_max_task_count));
        if (_current_task < _tasks.size())
            sendTask(worker_id);
        if (_free_workers_count == _workers.size()){
            stopAllThreads();
        }
    }
}

void QThreadController::pageLoaded(QString header, QString page, QVector<QPoint> positions)
{
    if (!_search_all && !positions.isEmpty())
        stopAllThreads();
    emit textFound(header, page, positions);
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

