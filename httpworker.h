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
#include <QDebug>


class QHttpWorker : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isPaused  READ getPaused  WRITE setPause)
    Q_PROPERTY(bool isWorking READ getWorking WRITE setWorking)
    Q_PROPERTY(int workerId READ getId WRITE setId)


public:
    explicit QHttpWorker(QObject *parent = 0);
    ~QHttpWorker();
    bool getPaused() const {QMutexLocker locker(&_pause_mutex); return _isPaused;}
    bool getWorking() const {QMutexLocker locker(&_pause_mutex); return _isWorking;}
    int getId() const {QMutexLocker locker(&_pause_mutex); return _id;}
    void setId(int id) {QMutexLocker locker(&_pause_mutex); _id = id;}
    void setCaseSensitive(bool cs) {QMutexLocker locker(&_pause_mutex); _case_sens =(cs? Qt::CaseSensitive : Qt::CaseInsensitive) ;}
    QString getPage() const {QMutexLocker locker(&_pause_mutex); return _page;}

signals:
    void searchFinished(QString header, QString page, QVector<QPoint> positions);
    void urlFound(QUrl url);
    void finished();
     void requestWork(int id);

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
    void setWorking(bool working_state) {QMutexLocker locker(&_pause_mutex); _isWorking = working_state;}
    void findUrls();
    void findText();

    bool _isWorking;
    bool _isPaused;
    Qt::CaseSensitivity  _case_sens;
    QSemaphore _pause_sem;
    mutable QMutex _pause_mutex;
    QNetworkAccessManager * _namanager;
    QVector<QPoint> * _found_pos;
    int _id;
    QString _text;
    QUrl _url;
    QString _page;
};

#endif // HTTPWORKER_H
