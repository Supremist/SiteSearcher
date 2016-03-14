#ifndef HTTPWORKER_H
#define HTTPWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTextCodec>
#include <QStringList>
#include <QRegExp>
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>
#include <QVector>
#include <QPoint>
#include "qtasklist.h"
#include <QDebug>


class QHttpWorker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPaused  READ getPaused  WRITE setPause)
    Q_PROPERTY(bool isWorking READ getWorking WRITE setWorking)

public:
    explicit QHttpWorker(QObject *parent = 0);
    bool setTaskList(QTaskList * tasks);
    bool getPaused() const {QMutexLocker locker(&_pause_mutex); return _isPaused;}
    bool getWorking() const {QMutexLocker locker(&_pause_mutex); return _isWorking;}
    QString getPage() const {QMutexLocker locker(&_pause_mutex); return _page;}

signals:
    void searchFinished(QString header, QString page, QVector<QPoint> positions);
    void urlFound(QUrl url);
    void finished();

public slots:
    void start();
    void startSearch(QUrl url, QString text);
    void pause();
    void resume();
    void stop();
    void setPause(bool pause_state) {if (pause_state) pause(); else resume();}

private slots:
    void replyRecived(QNetworkReply * reply);

private:
    void requestWork();
    void setWorking(bool working_state) {QMutexLocker locker(&_pause_mutex); _isWorking = working_state;}
    void findUrls();
    void findText();

    bool _isWorking;
    bool _isPaused;
    QTaskList * _tasks;
    QSemaphore _pause_sem;
    mutable QMutex _pause_mutex;
    QNetworkAccessManager * _namanager;
    QVector<QPoint> * _found_pos;
    QString _text;
    QUrl _url;
    QString _page;
};

#endif // HTTPWORKER_H
