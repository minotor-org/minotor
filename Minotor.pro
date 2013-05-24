#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T15:39:20
#
#-------------------------------------------------

QT       += core gui

TARGET = Minotor
TEMPLATE = app


SOURCES += \
    configdialog.cpp \
    easingcurvedreal.cpp \
    externalmasterview.cpp \
    ledmatrix.cpp \
    main.cpp\
    mainwindow.cpp \
    midi.cpp \
    midicontrol.cpp \
    midicontrollablelist.cpp \
    midicontrollableparameter.cpp \
    midicontrollablereal.cpp \
    midiinterface.cpp \
    midimapper.cpp \
    midimapping.cpp \
    minaballs.cpp \
    minabarsfromsides.cpp \
    minacurve.cpp \
    minadebug.cpp \
    minaexpandingobjects.cpp \
    minafallingobjects.cpp \
    minaflash.cpp \
    minaflashbars.cpp \
    minagradient.cpp \
    minaimage.cpp \
    minaplasma.cpp \
    minarainbowoil.cpp \
    minarandompixels.cpp \
    minarotatingbars.cpp \
    minastars.cpp \
    minatext.cpp \
    minavibration.cpp \
    minawaveform.cpp \
    minoanimation.cpp \
    minoanimationgroup.cpp \
    minoclocksource.cpp \
    minocontrol.cpp \
    minoitemizedproperty.cpp \
    minomaster.cpp \
    minopersistentobject.cpp \
    minopersistentobjectfactory.cpp \
    minoprogram.cpp \
    minoprogrambank.cpp \
    minoproperty.cpp \
    minopropertybeat.cpp \
    minopropertycolor.cpp \
    minopropertyeasingcurve.cpp \
    minopropertyfilename.cpp \
    minopropertyreal.cpp \
    minopropertytext.cpp \
    minotor.cpp \
    minotrigger.cpp \
    miprobnzichru.cpp \
    miprodebug.cpp \
    mipromatrix.cpp \
    miprosecondlives.cpp \
    miprowaves.cpp \
    uianimation.cpp \
    uianimationdescription.cpp \
    uianimationgroup.cpp \
    uianimationpicker.cpp \
    uianimationproperty.cpp \
    uieasingcurve.cpp \
    uiknob.cpp \
    uimaster.cpp \
    uimasteranimationgroup.cpp \
    uimastercontrol.cpp \
    uimidicontrollableparameter.cpp \
    uimidiinterface.cpp \
    uiprogram.cpp \
    uiprogrambank.cpp \
    uiprogrameditor.cpp \
    uiprogramview.cpp


HEADERS  += \
    configdialog.h \
    easingcurvedreal.h \
    externalmasterview.h \
    ledmatrix.h \
    mainwindow.h \
    midi.h \
    midicontrol.h \
    midicontrollablelist.h \
    midicontrollableparameter.h \
    midicontrollablereal.h \
    midiinterface.h \
    midimapper.h \
    midimapping.h \
    minaballs.h \
    minabarsfromsides.h \
    minacurve.h \
    minadebug.h \
    minaexpandingobjects.h \
    minafallingobjects.h \
    minaflash.h \
    minaflashbars.h \
    minagradient.h \
    minaimage.h \
    minaplasma.h \
    minarainbowoil.h \
    minarandompixels.h \
    minarotatingbars.h \    
    minastars.h \
    minatext.h \
    minavibration.h \
    minawaveform.h \
    minoanimation.h \
    minoanimationgroup.h \
    minoclocksource.h \
    minocontrol.h \
    minoitemizedproperty.h \
    minomaster.h \
    minopersistentobject.h \
    minopersistentobjectfactory.h \
    minoprogram.h \
    minoprogrambank.h \
    minoproperty.h \
    minopropertybeat.h \
    minopropertycolor.h \
    minopropertyeasingcurve.h \
    minopropertyfilename.h \
    minopropertyreal.h \
    minopropertytext.h \
    minotor.h \
    minotrigger.h \
    miprobnzichru.h \
    miprodebug.h \
    mipromatrix.h \
    miprosecondlives.h \
    miprowaves.h \
    uianimation.h \
    uianimationdescription.h \
    uianimationgroup.h \
    uianimationpicker.h \
    uianimationproperty.h \
    uieasingcurve.h \
    uiknob.h \
    uimaster.h \
    uimasteranimationgroup.h \
    uimastercontrol.h \
    uimidicontrollableparameter.h \
    uimidiinterface.h \
    uiprogram.h \
    uiprogrambank.h \
    uiprogrameditor.h \
    uiprogramview.h

FORMS    += \
    mainwindow.ui \
    configdialog.ui \
    externalmasterview.ui

include(libraries/qextserialport/src/qextserialport.pri)
include(libraries/rtmidi/rtmidi.pri)

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

cppcheck.commands = \
	cppcheck --quiet --enable=all \
	--force --inconclusive \
	-i libraries \
	-I libraries/qextserialport/src \
	./

QMAKE_EXTRA_TARGETS += clang qmake_clang cppcheck
