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
    minoanimation.cpp \
    minoanimationrandompixels.cpp \
    minoanimationexpandingobjects.cpp \
    configdialog.cpp \
    midimapping.cpp \
    midicontrol.cpp \
    minoanimationproperty.cpp \
    uidial.cpp \
    uianimation.cpp \
    minoanimationdebug.cpp \
    minochannel.cpp \
    uichannel.cpp \
    uipixeledview.cpp \
    uimastermonitor.cpp \
    minomaster.cpp \
    uimaster.cpp

HEADERS  += mainwindow.h \
    ledmatrix.h \
    midi.h \
    minotor.h \
    minoanimation.h \
    minoanimationrandompixels.h \
    minoanimationexpandingobjects.h \
    configdialog.h \
    midimapping.h \
    midicontrol.h \
    minoanimationproperty.h \
    uidial.h \
    uianimation.h \
    minoanimationdebug.h \
    minochannel.h \
    uichannel.h \
    uipixeledview.h \
    uimastermonitor.h \
    minomaster.h \
    uimaster.h

FORMS    += mainwindow.ui \
    configdialog.ui

include(libraries/qextserialport/src/qextserialport.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
