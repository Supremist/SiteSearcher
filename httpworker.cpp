#include "httpworker.h"

QHttpWorker::QHttpWorker(QObject *parent) : QObject(parent),
    _isWorking(false),
    _isPaused(true)
{

}

void QHttpWorker::start()
{
    if (!_isWorking)
        emit requestWork();
}


void QHttpWorker::startSearch(QUrl url, QString text)
{
    if (!_isWorking){
        resume();
        _text = text;
        QNetworkAccessManager * _namanager = new QNetworkAccessManager();
        connect(_namanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyRecived(QNetworkReply*)));
        qDebug()<<"GET send";
        _namanager->get(QNetworkRequest(url));

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
    if (_isPaused){
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
    if (_isPaused)
        _pause_sem.acquire();
    QByteArray content = reply->readAll();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    _page = codec->toUnicode(content.data());  //.split("\n");
    qDebug()<<"Reply recived";
    findUrls();
    findText();
    _isWorking = false;
    emit pageLoaded(_page);
    qDebug()<<"Requesting work";
    emit requestWork();
}

void QHttpWorker::findUrls()
{
    QRegExp url_reqexp ("(http://[!#$&-;=?-\\[\\]_a-z~]+)");
    int pos = 0;
    while ((pos = url_reqexp.indexIn(_page, pos)) != -1) {
        if (_isPaused)
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
        if (_isPaused)
            _pause_sem.acquire();
        emit textFound(pos);
        pos += ln;
    }
}

