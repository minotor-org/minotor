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
    minomaster.cpp \
    uimaster.cpp \
    minoanimationwaveform.cpp \
    minoanimationbarsfromsides.cpp \
    minoanimationplasma.cpp \
    minoclocksource.cpp \
    minocue.cpp \
    uichannelview.cpp \
    uicue.cpp \
    uichanneleditor.cpp \
    minoanimationfactory.cpp \
    uianimationdescription.cpp \
    uianimationpicker.cpp

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
    minomaster.h \
    uimaster.h \
    minoanimationwaveform.h \
    minoanimationbarsfromsides.h \
    minoanimationplasma.h \
    minoclocksource.h \
    minocue.h \
    uichannelview.h \
    uicue.h \
    uichanneleditor.h \
    minoanimationfactory.h \
    uianimationdescription.h \
    uianimationpicker.h

FORMS    += mainwindow.ui \
    configdialog.ui

include(libraries/qextserialport/src/qextserialport.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
unix: CONFIG += extserialport

RESOURCES += \
    minotor.qrc
