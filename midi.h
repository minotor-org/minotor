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
    QStringList getPorts();

    void openPort(const unsigned int index);

    // Should not be used by user...
    void midiCallback( double deltatime, std::vector< unsigned char > *message);
private:
    RtMidiIn *_midiIn;

signals:
    void clockReceived();
    void startReceived();
    void stopReceived();
    void continueReceived();
    void controlChanged(quint8 number, quint8 value);

public slots:
    
};

#endif // MIDI_H
