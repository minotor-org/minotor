#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T15:39:20
#
#-------------------------------------------------

QT       += core gui

TARGET = Minotor
TEMPLATE = app


SOURCES += \
    Ui/Widget/uianimation.cpp \
    Ui/Widget/uianimationdescription.cpp \
    Ui/Widget/uianimationgroup.cpp \
    Ui/Widget/uianimationpicker.cpp \
    Ui/Widget/uianimationproperty.cpp \
    Ui/Widget/uieasingcurve.cpp \
    Ui/Widget/uiknob.cpp \
    Ui/Widget/uimaster.cpp \
    Ui/Widget/uimasteranimationgroup.cpp \
    Ui/Widget/uimastercontrol.cpp \
    Ui/Widget/uimidicontrollableparameter.cpp \
    Ui/Widget/uimidiinterface.cpp \
    Ui/Widget/uiprogram.cpp \
    Ui/Widget/uiprogrambank.cpp \
    Ui/Widget/uiprogrameditor.cpp \
    Ui/Widget/uiprogramview.cpp \
    Ui/configdialog.cpp \
    Ui/externalmasterview.cpp \
    Ui/mainwindow.cpp \
    easingcurvedreal.cpp \
    ledmatrix.cpp \
    main.cpp\
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
    minomastermidimapper.cpp \
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
    miprowaves.cpp


HEADERS  += \
    Ui/Widget/uianimation.h \
    Ui/Widget/uianimationdescription.h \
    Ui/Widget/uianimationgroup.h \
    Ui/Widget/uianimationpicker.h \
    Ui/Widget/uianimationproperty.h \
    Ui/Widget/uieasingcurve.h \
    Ui/Widget/uiknob.h \
    Ui/Widget/uimaster.h \
    Ui/Widget/uimasteranimationgroup.h \
    Ui/Widget/uimastercontrol.h \
    Ui/Widget/uimidicontrollableparameter.h \
    Ui/Widget/uimidiinterface.h \
    Ui/Widget/uiprogram.h \
    Ui/Widget/uiprogrambank.h \
    Ui/Widget/uiprogrameditor.h \
    Ui/Widget/uiprogramview.h \
    Ui/configdialog.h \
    Ui/externalmasterview.h \
    Ui/mainwindow.h \
    easingcurvedreal.h \
    ledmatrix.h \
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
    minomastermidimapper.h \
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
    miprowaves.h

INCLUDEPATH += \
    Ui \
    Ui/Widget

FORMS    += \
    Ui/mainwindow.ui \
    Ui/configdialog.ui \
    Ui/externalmasterview.ui

include(libraries/qextserialport/src/qextserialport.pri)
include(libraries/rtmidi/rtmidi.pri)

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += rtmidi
unix: CONFIG += extserialport

RESOURCES += \
    minotor.qrc

OTHER_FILES += \
    style.css

UI_DIR = Ui/

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
