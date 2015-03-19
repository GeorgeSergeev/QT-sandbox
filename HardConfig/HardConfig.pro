#-------------------------------------------------
#
# Project created by QtCreator 2015-03-16T17:09:15
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HardConfig
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    environmentinfo.cpp

HEADERS  += mainwindow.h \
    environmentinfo.h

FORMS    += mainwindow.ui
