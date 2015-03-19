#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T21:56:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClassNameTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    commands/command.cpp \
    commands/childcommand.cpp

HEADERS  += mainwindow.h \
    commands/command.h \
    commands/childcommand.h

FORMS    += mainwindow.ui
