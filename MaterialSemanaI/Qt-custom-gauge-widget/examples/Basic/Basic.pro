#-------------------------------------------------
#
# Project created by QtCreator 2014-11-20T19:11:44
#
#-------------------------------------------------

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Basic
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../source/qcgaugewidget.cpp \
    sockettest.cpp

HEADERS  += mainwindow.h \
    ../../source/qcgaugewidget.h \
    sockettest.h

FORMS    += mainwindow.ui

