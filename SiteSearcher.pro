#-------------------------------------------------
#
# Project created by QtCreator 2016-03-12T00:12:08
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SiteSearcher
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    threadcontroller.cpp \
    httpworker.cpp

HEADERS  += mainwindow.h \
    threadcontroller.h \
    httpworker.h

FORMS    += mainwindow.ui
