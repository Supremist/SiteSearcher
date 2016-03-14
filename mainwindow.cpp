#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isPaused(false),
    isSearching(false)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(btn1Clicked()));
    connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(btn2Clicked()));
    page_viewer.connectWithUI(ui->listWidget, ui->textBrowser);
    connect(ui->checkBox_3, SIGNAL(toggled(bool)), &page_viewer, SLOT(refreshHeaders(bool)));
    connect(&thread_controller, SIGNAL(textFound(QString,QString,QVector<QPoint>)),
            &page_viewer, SLOT(addPage(QString,QString,QVector<QPoint>)));
    connect(&thread_controller, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)));
    connect(&thread_controller, SIGNAL(searchFinished()), this, SLOT(searchFinished()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btn1Clicked()
{
    if (isSearching){
        thread_controller.stopAllThreads();
    }
    else{
        page_viewer.clear();
        thread_controller.startSearch(QUrl(ui->lineEdit->text()), ui->lineEdit_3->text(),
                         ui->spinBox->value(), ui->spinBox_2->value(),
                         ui->checkBox->isChecked(), ui->checkBox_2->isChecked());
        ui->pushButton->setText("Stop search");
        isSearching = true;
        ui->pushButton_2->setEnabled(true);
    }
}

void MainWindow::btn2Clicked()
{
    if (isPaused){
        thread_controller.resumeAllThreads();
        ui->pushButton_2->setText("Pause");
    }
    else{
        thread_controller.pauseAllThreads();
        ui->pushButton_2->setText("Resume");
    }
    isPaused = !isPaused;
}

void MainWindow::searchFinished()
{
    ui->pushButton_2->setText("Pause");
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setText("Start search");
    isPaused = false;
    isSearching = false;
}
