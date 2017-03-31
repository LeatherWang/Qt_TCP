#-------------------------------------------------
#
# Project created by QtCreator 2017-03-28T08:57:14
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = tcp_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += \
    mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
