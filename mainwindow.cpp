#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(btn1Clicked()));
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

}
