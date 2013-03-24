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
    minafallingobjects.cpp \
    minastars.cpp \
    minagradient.cpp \
    miprobnzichru.cpp \
    minoanimationgroup.cpp \
    uianimationgroup.cpp \
    miprodebug.cpp \
    externalmasterview.cpp \
    uiknob.cpp \
    uimasteranimationgroup.cpp \
    minavibration.cpp \
    minopersistentobject.cpp

HEADERS  += mainwindow.h \
    ledmatrix.h \
    midi.h \
    minotor.h \
    minoanimation.h \
    configdialog.h \
    midimapping.h \
    midicontrol.h \
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
    minafallingobjects.h \
    minastars.h \
    minagradient.h \
    miprobnzichru.h \
    minoanimationgroup.h \
    uianimationgroup.h \
    miprodebug.h \
    externalmasterview.h \
    uiknob.h \
    uimasteranimationgroup.h \
    minavibration.h \
    minopersistentobject.h

FORMS    += mainwindow.ui \
    configdialog.ui \
    mainwindowng.ui \
    externalmasterview.ui

include(libraries/qextserialport/src/qextserialport.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
unix: CONFIG += extserialport

RESOURCES += \
    minotor.qrc

OTHER_FILES += \
    style.css
