#ifndef MIDI_H
#define MIDI_H

#include <QObject>
#include <QStringList>

#include "RtMidi.h"

class Midi : public QObject
{
    Q_OBJECT
public:
    explicit Midi(QObject *parent = 0);

    // List ports
    QStringList getPorts();

    // Open port by index (index is relative to current port list's index)
    bool openPort(const unsigned int index);
    // Close open MIDI connection (if one exists).
    void closePort();

    // Test if MIDI is open
    bool isOpen();

    // Retreive current connected port name
    QString portName();

    // Warning: Should not be used by user...
    void midiCallback( double deltatime, std::vector< unsigned char > *message);
private:
    RtMidiIn *_midiIn;
    unsigned int _portIndex;
    bool _connected;

signals:
    void connected(bool connected = true);

    void clockReceived();
    void startReceived();
    void stopReceived();
    void continueReceived();
    void controlChanged(quint8 channel, quint8 number, quint8 value);

public slots:
    
};

typedef QList<Midi*> QMidiInterfaceList;

#endif // MIDI_H
