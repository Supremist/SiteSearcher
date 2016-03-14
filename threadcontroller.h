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
    void workRequested();

private:
    QVector<QThread *> _threads;
    QVector<QHttpWorker *> _workers;
    QTaskList * _tasks;
    QString _text;
    //QList<int>
};

#endif // THREADCONTROLLER_H
