#ifndef MIDI_H
#define MIDI_H

#include <QObject>
#include <QStringList>

#include "RtMidi.h"

class MidiInterface;
typedef QList<MidiInterface*> MidiInterfaces;

class Midi : public QObject
{
    Q_OBJECT

    friend class MidiInterface;
public:
    explicit Midi(QObject *parent = 0);
    ~Midi();

    // List ports
    QStringList getPorts();

    // Interfaces
    MidiInterfaces interfaces();
    MidiInterface* interface(QString portName);
    MidiInterface* addMidiInterface(QString portName);

private:
    int grabMidiInterfaceId();
    void addMidiInterface(MidiInterface *interface);

    RtMidiIn *_midiIn;
    MidiInterfaces _interfaces;

signals:
    void controlChanged(int interface, quint8 channel, quint8 control, quint8 value);
    void programChanged(int interface, quint8 channel, quint8 program);
    void noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value);

public slots:
    
};

#endif // MIDI_H
