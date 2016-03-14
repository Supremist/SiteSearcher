#include "httpworker.h"

QHttpWorker::QHttpWorker(QObject *parent) : QObject(parent),
    _isWorking(false),
    _isPaused(false),
    _namanager(nullptr),
    _found_pos(nullptr),
    _id(0)
{

}

QHttpWorker::~QHttpWorker()
{
    qDebug()<<_id<<" Destroyed";
}

void QHttpWorker::start()
{
    if (!_isWorking){
        if (!_namanager){
            _namanager = new QNetworkAccessManager();
            qDebug()<<_id<<"NA created";
            connect(_namanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyRecived(QNetworkReply*)));
        }
        if (!_found_pos){
            _found_pos = new QVector<QPoint>();
        }
        qRegisterMetaType<QVector<QPoint> >("QVector<QPoint>");
        //requestWork();
    }
}


void QHttpWorker::startSearch(QUrl url, QString text)
{
    if (!_isWorking){
        //resume();
        _isWorking = true;
        _text = text;
        _url = url;
        _found_pos->clear();
        qDebug()<<_id<<"GET send";
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
    qDebug()<<_id<<" Stopped";
    delete _namanager;
    delete _found_pos;
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
    setWorking (false);
    QString header = _url.toString() + " - " + QString::number(_found_pos->size()) + " matches";
    emit searchFinished(header, _page, *_found_pos);
    qDebug()<<"Requesting work";
    requestWork(_id);
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
    while ((pos = _page.indexOf(_text, pos, _case_sens)) != -1) {
        if (_isPaused)
            _pause_sem.acquire();
        _found_pos->append(QPoint(pos, _text.length()));
        pos += ln;
    }
}

