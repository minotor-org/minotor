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
    midimapper.cpp \
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
    uimidicontrollableparameter.cpp \
    minatext.cpp \
    minopropertytext.cpp \
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
    minopersistentobject.cpp \
    minopropertyreal.cpp \
    minarotatingbars.cpp \
    minacurve.cpp \
    miprosecondlives.cpp \
    midiinterface.cpp \
    uimidiinterface.cpp \
    minotrigger.cpp \
    minocontrol.cpp \
    minopropertycolor.cpp \
    midicontrollableparameter.cpp \
    midicontrollablereal.cpp \
    midicontrollablelist.cpp \
    minopropertyeasingcurve.cpp \
    minaballs.cpp \
    minaimage.cpp \
    minopropertyfilename.cpp \
    midimapping.cpp \
    uieasingcurve.cpp


HEADERS  += mainwindow.h \
    ledmatrix.h \
    midi.h \
    minotor.h \
    minoanimation.h \
    configdialog.h \
    midimapper.h \
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
    uimidicontrollableparameter.h \
    minatext.h \
    minopropertytext.h \
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
    minopersistentobject.h \
    minopropertyreal.h \
    minarotatingbars.h \    
    minacurve.h \
    miprosecondlives.h \
    midiinterface.h \
    uimidiinterface.h \
    minotrigger.h \
    minocontrol.h \
    minopropertycolor.h \
    midicontrollableparameter.h \
    midicontrollablereal.h \
    midicontrollablelist.h \
    minopropertyeasingcurve.h \
    minaballs.h \
    minaimage.h \
    minopropertyfilename.h \
    midimapping.h \
    uieasingcurve.h

FORMS    += mainwindow.ui \
    configdialog.ui \
    externalmasterview.ui

include(libraries/qextserialport/src/qextserialport.pri)
win32: include(libraries/rtmidi/rtmidi.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
unix: CONFIG += extserialport

RESOURCES += \
    minotor.qrc

OTHER_FILES += \
    style.css

qmake_clang.commands = qmake -recursive -spec unsupported/linux-clang
clang.commands = scan-build make clean all
clang.depends = qmake_clang

QMAKE_EXTRA_TARGETS += clang qmake_clang
