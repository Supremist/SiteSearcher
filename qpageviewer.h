#ifndef QPAGEVIEWER_H
#define QPAGEVIEWER_H

#include <QObject>
#include <QListWidget>
#include <QTextBrowser>
#include <QTextCursor>
#include <QStringList>
#include <QPoint>
#include <QVector>

class QPageViewer : public QObject
{
    Q_OBJECT
public:
    explicit QPageViewer(QObject *parent = 0);
    void connectWithUI(QListWidget * list_widget, QTextBrowser * text_browser);
    void disconnectUI();

signals:

public slots:
    void viewPage(int index);
    void clear();
    void addPage(QString header, QString page, QVector<QPoint> highlighted = QVector<QPoint>());
    void refreshHeaders();

private:
    void highlightText(const int start_pos, const int length);
    QListWidget * _list_widget;
    QTextBrowser * _text_browser;
    QStringList _pages;
    QStringList _headers;
    QVector < QVector<QPoint> > _highlighted;
};

#endif // QPAGEVIEWER_H
