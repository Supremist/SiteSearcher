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
#include <QDebug>

//enum class WorkerState {
//    Working,
//    Waiting,
//    Paused
//};

class Position{
public:
    Position(int line_, int pos_):
        line(line_),
        pos(pos_)
    {}
    int line;
    int pos;
};

class QHttpWorker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPaused  READ getPaused  WRITE setPause)
    Q_PROPERTY(bool isWorking READ getWorking WRITE setWorking)

public:
    explicit QHttpWorker(QObject *parent = 0);
    bool getPaused() const {QMutexLocker locker(&_pause_mutex); return _isPaused;}
    bool getWorking() const {QMutexLocker locker(&_pause_mutex); return _isWorking;}

signals:
    void requestWork();
    //void searchFinished(QList<QUrl> urls, QStringList page, QList<Position> positions);
    void urlFound(QUrl url);
    void textFound(int pos);
    void finished();

public slots:
    void start();
    void startSearch(QUrl url, QString text);
    void pause();
    void resume();
    void stop();
    void setPaused(bool pause_state) {if (pause_state) pause(); else resume();}

private slots:
    void replyRecived(QNetworkReply * reply);

private:
    bool setWorking(bool working_state) {QMutexLocker locker(&_pause_mutex); _isWorking = working_state;}
    void findUrls();
    void findText();

    //WorkerState _state;
    bool _isWorking;
    bool _isPaused;
    QSemaphore _pause_sem;
    mutable QMutex _pause_mutex;
    QNetworkAccessManager _namanager;
    QString _text;
    QString _page;
};

#endif // HTTPWORKER_H
