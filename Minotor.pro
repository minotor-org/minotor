#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T15:39:20
#
#-------------------------------------------------

QT       += core gui

TARGET = Minotor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ledmatrix.cpp \
    midi.cpp \
    minotor.cpp \
    monitor.cpp

HEADERS  += mainwindow.h \
    ledmatrix.h \
    midi.h \
    minotor.h \
    monitor.h

FORMS    += mainwindow.ui

include(libraries/qextserialport/src/qextserialport.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
