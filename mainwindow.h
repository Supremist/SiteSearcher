#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "threadcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void btn1Clicked();

private:
    Ui::MainWindow *ui;
    QHttpWorker _worker;
    QThreadController _cnt;
};

#endif // MAINWINDOW_H
