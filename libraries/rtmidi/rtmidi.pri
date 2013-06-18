exists("$$PWD/RtMidi.cpp") {
  INCLUDEPATH += $$PWD
  DEPENDPATH += $$PWD

  PUBLIC_HEADERS         += $$PWD/RtMidi.h \
                          $$PWD/RtError.h

  HEADERS                += $$PUBLIC_HEADERS

  SOURCES                += $$PWD/RtMidi.cpp

  win32:DEFINES          +=  __WINDOWS_MM__
  win32:LIBS             += -lwinmm

  unix:DEFINES           += __LINUX_ALSA__
  unix:LIBS              += -lasound
}

!exists("$$PWD/RtMidi.cpp") {
  # Attempt to use pkg-config to find an available rtmidi on system
  unix: PKGCONFIG += rtmidi
}
