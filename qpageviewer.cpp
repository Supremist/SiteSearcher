#include "qpageviewer.h"

QPageViewer::QPageViewer(QObject *parent) : QObject(parent),
    _show_all(false),
    _list_widget(nullptr),
    _text_browser(nullptr)
{

}

void QPageViewer::connectWithUI(QListWidget *list_widget, QTextBrowser *text_browser)
{
    disconnectUI();
    _list_widget = list_widget;
    _text_browser = text_browser;
    connect(_list_widget, SIGNAL(currentRowChanged(int)), this, SLOT(viewPage(int)));
}

void QPageViewer::disconnectUI()
{
    if (_list_widget)
        disconnect(_list_widget, SIGNAL(currentRowChanged(int)), this, SLOT(viewPage(int)));
}

void QPageViewer::viewPage(int index)
{
    if (_text_browser && _list_widget){
        if (index < 0 ){
            _text_browser->clear();
        }
        else if(index < _headers.size()){
            if(_list_widget->currentRow() != index)
                _list_widget->setCurrentRow(index);
            _text_browser->clear();
            _text_browser->setTextBackgroundColor(Qt::white);

            if (!_show_all){
                int i = 0;
                while(i<_highlighted.size() && index >= 0){
                    if (!_highlighted[i].isEmpty())
                        index--;
                    i++;
                }
                index = i-1;
            }

            _text_browser->setPlainText(_pages[index]);
            foreach (const QPoint & selection, _highlighted[index]) {
                highlightText(selection.x(), selection.y());
            }
        }
    }
}

void QPageViewer::clear()
{
    _headers.clear();
    _pages.clear();
    _highlighted.clear();
    if (_text_browser && _list_widget){
        _list_widget->clear();
        _text_browser->clear();
    }
}

void QPageViewer::addPage(QString header, QString page, QVector<QPoint> highlighted)
{
    _headers.append(header);
    _pages.append(page);
    _highlighted.append(highlighted);
    if(_list_widget ){
        if(_show_all || !highlighted.isEmpty())
            _list_widget->addItem(header);
    }
}

void QPageViewer::refreshHeaders(bool show_all)
{
    if(_list_widget){
        _show_all = show_all;
        int row = _list_widget->currentRow();
        _list_widget->clear();
        if (show_all){
            _list_widget->addItems(_headers);
            _list_widget->setCurrentRow(row);
        }
        else{
            _list_widget->setCurrentRow(-1);
            for(int i = 0; i<_headers.size(); ++i){
                if (!_highlighted[i].isEmpty())
                    _list_widget->addItem(_headers[i]);
            }
        }
    }
}

void QPageViewer::highlightText(const int start_pos, const int length)
{
    QTextCharFormat fmt;
    fmt.setBackground(Qt::yellow);

    QTextCursor cursor(_text_browser->document());
    cursor.setPosition(start_pos, QTextCursor::MoveAnchor);
    cursor.setPosition(start_pos+length, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
}

