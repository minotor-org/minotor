#-------------------------------------------------
#
# Project created by QtCreator 2012-12-17T15:39:20
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minotor
TEMPLATE = app


SOURCES += \
    Animation/minaballs.cpp \
    Animation/minabarsfromsides.cpp \
    Animation/minacurve.cpp \
    Animation/minadebug.cpp \
    Animation/minaexpandingobjects.cpp \
    Animation/minafallingobjects.cpp \
    Animation/minaflash.cpp \
    Animation/minaflashbars.cpp \
    Animation/minagradient.cpp \
    Animation/minaimage.cpp \
    Animation/minaplasma.cpp \
    Animation/minarainbowoil.cpp \
    Animation/minarandompixels.cpp \
    Animation/minarotatingbars.cpp \
    Animation/minastars.cpp \
    Animation/minatext.cpp \
    Animation/minavibration.cpp \
    Animation/minawaveform.cpp \
    Core/artdmx.cpp \
    Core/Midi/midi.cpp \
    Core/Midi/midicontrol.cpp \
    Core/Midi/midicontrollablelist.cpp \
    Core/Midi/midicontrollableparameter.cpp \
    Core/Midi/midicontrollablereal.cpp \
    Core/Midi/midiinterface.cpp \
    Core/Midi/midimapper.cpp \
    Core/Midi/midimapping.cpp \
    Core/Property/minoitemizedproperty.cpp \
    Core/Property/minoproperty.cpp \
    Core/Property/minopropertybeat.cpp \
    Core/Property/minopropertycolor.cpp \
    Core/Property/minopropertyeasingcurve.cpp \
    Core/Property/minopropertyfilename.cpp \
    Core/Property/minopropertyreal.cpp \
    Core/Property/minopropertytext.cpp \
    Core/easingcurvedreal.cpp \
    Core/ledmatrix.cpp \
    Core/minoanimation.cpp \
    Core/minoanimationgroup.cpp \
    Core/minoclocksource.cpp \
    Core/minocontrol.cpp \
    Core/minoinstrumentedanimation.cpp \
    Core/minomaster.cpp \
    Core/minomastermidimapper.cpp \
    Core/minopersistentobject.cpp \
    Core/minopersistentobjectfactory.cpp \
    Core/minoprogram.cpp \
    Core/minoprogrambank.cpp \
    Core/minopropertymidichannel.cpp \
    Core/minotor.cpp \
    Core/minotrigger.cpp \
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
    main.cpp\
    miprodebug.cpp \
    Animation/minagrid.cpp


HEADERS  += \
    Animation/minaballs.h \
    Animation/minabarsfromsides.h \
    Animation/minacurve.h \
    Animation/minadebug.h \
    Animation/minaexpandingobjects.h \
    Animation/minafallingobjects.h \
    Animation/minaflash.h \
    Animation/minaflashbars.h \
    Animation/minagradient.h \
    Animation/minaimage.h \
    Animation/minaplasma.h \
    Animation/minarainbowoil.h \
    Animation/minarandompixels.h \
    Animation/minarotatingbars.h \    
    Animation/minastars.h \
    Animation/minatext.h \
    Animation/minavibration.h \
    Animation/minawaveform.h \
    Core/artdmx.h \
    Core/Midi/midi.h \
    Core/Midi/midicontrol.h \
    Core/Midi/midicontrollablelist.h \
    Core/Midi/midicontrollableparameter.h \
    Core/Midi/midicontrollablereal.h \
    Core/Midi/midiinterface.h \
    Core/Midi/midimapper.h \
    Core/Midi/midimapping.h \
    Core/Property/minoitemizedproperty.h \
    Core/Property/minoproperty.h \
    Core/Property/minopropertybeat.h \
    Core/Property/minopropertycolor.h \
    Core/Property/minopropertyeasingcurve.h \
    Core/Property/minopropertyfilename.h \
    Core/Property/minopropertyreal.h \
    Core/Property/minopropertytext.h \
    Core/easingcurvedreal.h \
    Core/ledmatrix.h \
    Core/minoanimation.h \
    Core/minoanimationgroup.h \
    Core/minoclocksource.h \
    Core/minocontrol.h \
    Core/minoinstrumentedanimation.h \
    Core/minomaster.h \
    Core/minomastermidimapper.h \
    Core/minopersistentobject.h \
    Core/minopersistentobjectfactory.h \
    Core/minoprogram.h \
    Core/minoprogrambank.h \
    Core/minopropertymidichannel.h \
    Core/minotor.h \
    Core/minotrigger.h \
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
    miprobnzichru.h \
    miprodebug.h \
    mipromatrix.h \
    miprosecondlives.h \
    miprowaves.h \
    Animation/minagrid.h

INCLUDEPATH += \
    Animation \
    Core \
    Core/Midi \
    Core/Property \
    Ui \
    Ui/Widget

FORMS    += \
    Ui/mainwindow.ui \
    Ui/configdialog.ui \
    Ui/externalmasterview.ui

packagesExist(rtmidi) {
  PKGCONFIG += rtmidi
} else {
  include(libraries/rtmidi/rtmidi.pri)
}

unix {
  CONFIG += link_pkgconfig
  CONFIG += extserialport
} else {
  include(libraries/qextserialport/src/qextserialport.pri)
}


RESOURCES += \
    minotor.qrc

# Windows icon
RC_FILE = minotor.rc

# MacOS icon
# Note: to rebuild icns, on MacOS
# $ iconutil -c icns -o minotor.icns minotor.iconset
ICON = minotor.icns

dmg.commands = macdeployqt minotor.app/ -dmg

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

QMAKE_EXTRA_TARGETS += clang qmake_clang cppcheck dmg

unix {
  #VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL

  INSTALLS += target desktop icon

  target.path =$$BINDIR

  desktop.path = $$DATADIR/applications
  desktop.files += $${TARGET}.desktop

  icon.path = $$DATADIR/pixmaps
  icon.files += $${TARGET}.xpm

#  icon64.path = $$DATADIR/icons/hicolor/64x64/apps
#  icon64.files += ../data/64x64/$${TARGET}.png
}
