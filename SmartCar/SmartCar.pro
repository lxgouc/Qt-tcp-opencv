#-------------------------------------------------
#
# Project created by QtCreator 2017-08-22T13:05:46
#
#-------------------------------------------------

QT       += core gui

QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartCar
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    newqlabel.cpp

HEADERS  += widget.h \
    newqlabel.h

FORMS    += widget.ui

INCLUDEPATH   += /usr/local/include/opencv \
                 /usr/local/include  \
                 /udt/local/include/opencv2


LIBS      +=   `pkg-config opencv --cflags --libs`
