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
    configdialog.cpp \
    midimapping.cpp \
    midicontrol.cpp \
    uidial.cpp \
    uianimation.cpp \
    minomaster.cpp \
    uimaster.cpp \
    minoclocksource.cpp \
    minoanimationfactory.cpp \
    uianimationdescription.cpp \
    uianimationpicker.cpp \
    minabarsfromsides.cpp \
    minaexpandingobjects.cpp \
    minaplasma.cpp \
    minarandompixels.cpp \
    minawaveform.cpp \
    uianimationproperty.cpp \
    minoproperty.cpp \
    minoitemizedproperty.cpp \
    minoprogram.cpp \
    uiprogrameditor.cpp \
    mipromatrix.cpp \
    minaflash.cpp \
    uimastercontrol.cpp \
    uiprogrambank.cpp \
    uiprogram.cpp \
    uiprogramview.cpp \
    miprowaves.cpp \
    minadebug.cpp \
    uimidiproperty.cpp \
    minatext.cpp \
    minotextproperty.cpp \
    miprotext.cpp \
    minafallingobjects.cpp \
    minastars.cpp \
    minagradient.cpp

HEADERS  += mainwindow.h \
    ledmatrix.h \
    midi.h \
    minotor.h \
    minoanimation.h \
    configdialog.h \
    midimapping.h \
    midicontrol.h \
    uidial.h \
    uianimation.h \
    minomaster.h \
    uimaster.h \
    minoclocksource.h \
    minoanimationfactory.h \
    uianimationdescription.h \
    uianimationpicker.h \
    minabarsfromsides.h \
    minaexpandingobjects.h \
    minaplasma.h \
    minarandompixels.h \
    minawaveform.h \
    uianimationproperty.h \
    minoproperty.h \
    minoitemizedproperty.h \
    minoprogram.h \
    uiprogrameditor.h \
    uiprogramview.h \
    mipromatrix.h \
    minaflash.h \
    uimastercontrol.h \
    uiprogrambank.h \
    uiprogram.h \
    miprowaves.h \
    minadebug.h \
    uimidiproperty.h \
    minatext.h \
    minotextproperty.h \
    miprotext.h \
    minafallingobjects.h \
    minastars.h \
    minagradient.h

FORMS    += mainwindow.ui \
    configdialog.ui \
    mainwindowng.ui

include(libraries/qextserialport/src/qextserialport.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
unix: CONFIG += extserialport

RESOURCES += \
    minotor.qrc

OTHER_FILES += \
    style.css
