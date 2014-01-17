exists("$$PWD/RtMidi.cpp") {
  INCLUDEPATH += $$PWD
  DEPENDPATH += $$PWD

  PUBLIC_HEADERS         += $$PWD/RtMidi.h \
                          $$PWD/RtError.h

  HEADERS                += $$PUBLIC_HEADERS

  SOURCES                += $$PWD/RtMidi.cpp

  win32:DEFINES          +=  __WINDOWS_MM__
  win32:LIBS             += -lwinmm

  unix:!macx:DEFINES     += __LINUX_ALSA__
  unix:!macx:LIBS        += -lasound

  unix:macx:DEFINES      += __MACOSX_CORE__
  unix:macx:LIBS         += -framework CoreMIDI -framework CoreFoundation -framework CoreAudio
}

!exists("$$PWD/RtMidi.cpp") {
  # Attempt to use pkg-config to find an available rtmidi on system
  unix: PKGCONFIG += rtmidi
}
