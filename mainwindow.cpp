#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(btn1Clicked()));
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(btn2Clicked()));
    vw.connectWithUI(ui->listWidget, ui->textBrowser);
    connect(&_cnt, SIGNAL(textFound(QString,QString,QVector<QPoint>)), &vw, SLOT(addPage(QString,QString,QVector<QPoint>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btn1Clicked()
{
    //_worker.searchUrl(QUrl("http://csb.stanford.edu/class/public/pages/sykes_webdesign/05_simple.html"), QString("text"));
    //_worker.searchUrl(QUrl("http://www.google.com.ua/"), QString("text"));
    //QStringList lst = QString("error,httptest sldoec,ksdlkfhttp://tss ssdsdf, sdscshttp://stackoverflow.com/questions/5999967/get-plain-text-from-qstring-with-html-tags<> sdls").split(',');
    //_worker.findUrls(lst);
    vw.clear();
    _cnt.startSearch(QUrl("http://www.google.com.ua/"), QString("<haed>"), 10, 5);
}

void MainWindow::btn2Clicked()
{

}
