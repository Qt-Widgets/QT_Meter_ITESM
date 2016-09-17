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

#QMAKE_LFLAGS += --sysroot

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../source/qcgaugewidget.cpp \
    clientconnection.cpp

HEADERS  += mainwindow.h \
    ../../source/qcgaugewidget.h \
    clientconnection.h

FORMS    += mainwindow.ui

