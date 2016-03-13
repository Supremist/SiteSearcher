#include "httpworker.h"

QHttpWorker::QHttpWorker(QObject *parent) : QObject(parent),
    _isWorking(false),
    _isPaused(true)
{
    connect(&_namanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyRecived(QNetworkReply*)));
}

void QHttpWorker::start()
{
    if (!isWorking)
        emit requestWork();
}


void QHttpWorker::startSearch(QUrl url, QString text)
{
    if (!isWorking){
        resume();
        _text = text;
        _namanager.get(QNetworkRequest(url));

    }
}

void QHttpWorker::pause()
{
    QMutexLocker locker(&_pause_mutex);
    _isPaused = true;
}

void QHttpWorker::resume()
{
    QMutexLocker locker(&_pause_mutex);
    if (isPaused){
        _pause_sem.release();
        _isPaused = false;
    }
}

void QHttpWorker::stop()
{
    emit finished();
}

void QHttpWorker::replyRecived(QNetworkReply *reply)
{
    if (isPaused)
        _pause_sem.acquire();
    QByteArray content = reply->readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    _page = codec->toUnicode(content.data());  //.split("\n");
    findUrls();
    findText();
    isWorking = false;
    emit requestWork();
}

void QHttpWorker::findUrls()
{
    QRegExp url_reqexp ("(http://[!#$&-;=?-\\[\\]_a-z~]+)");
    int pos = 0;
    while ((pos = url_reqexp.indexIn(_page, pos)) != -1) {
        if (isPaused)
            _pause_sem.acquire();
        emit urlFound(QUrl(url_reqexp.cap(1)));
        pos += url_reqexp.matchedLength();
    }
}

void QHttpWorker::findText()
{
    int pos = 0;
    int ln = _text.length();
    while ((pos = _page.indexOf(_text, pos)) != -1) {
        if (isPaused)
            _pause_sem.acquire();
        emit textFound(pos);
        pos += ln;
    }
}

