#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(btn1Clicked()));
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(btn2Clicked()));
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
    _cnt.startSearch(QUrl("http://www.sheldonbrown.com/web_sample1.html"), QString("text"), 1);
}

void MainWindow::btn2Clicked()
{
    vw.connectWithUI(ui->listWidget, ui->textBrowser);
    vw.clear();
    vw.addPage("first header", "test page");
    QVector<QPoint> vc;
    vc.append(QPoint(6, 10));
    vc.append(QPoint(20, 5));
    vw.addPage("sec header", "test page\n second line");
    vw.addPage("3 header", "test page\n second line\n\n 4line", vc);
}
