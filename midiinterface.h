#ifndef MIDIINTERFACE_H
#define MIDIINTERFACE_H

#include <QObject>

#include "RtMidi.h"

class Midi;

class MidiInterface : public QObject
{
    Q_OBJECT

    friend class Midi;
public:
    explicit MidiInterface(QString portName, QObject *parent);
    ~MidiInterface();

    // List ports
    QStringList getPorts();

    // Open port using _portName
    bool open();
    // Open port by name
    bool open(QString portName);

    // Close open MIDI connection (if one exists).
    void close();

    // Test if MIDI is open
    bool isConnected();

    // Retreive current connected port name
    QString portName();

    // ID
    int id() { return _id; }

    // RtMidi callback
    // Warning: Should not be used by user...
    void midiCallback( double deltatime, std::vector< unsigned char > *message);
private:
    RtMidiIn *_rtMidiIn;
    int _id;
    unsigned int _portIndex;
    QString _portName;
    bool _connected;

    // Special accessor: we use objectName to store portName
    void setPortName(QString portName);

    // Open port by index (index is relative to current port list's index)
    bool open(const unsigned int index);

signals:
    void connected(bool connected = true);
    void clockReceived();
    void startReceived();
    void stopReceived();
    void continueReceived();
    void controlChanged(int interface, quint8 channel, quint8 control, quint8 value);
    void programChanged(int interface, quint8 channel, quint8 program);
    void noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value);

public slots:
    
};

#endif // MIDIINTERFACE_H
