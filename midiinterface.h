#ifndef MIDIINTERFACE_H
#define MIDIINTERFACE_H

#include <QObject>

#include "RtMidi.h"

class Midi;

class MidiInterface : public QObject
{
    Q_OBJECT

    friend class Midi;

    Q_PROPERTY(QString name READ portName)
    Q_PROPERTY(bool accept_clock READ acceptClock WRITE setAcceptClock)
    Q_PROPERTY(bool accept_program READ acceptProgramChange WRITE setAcceptProgramChange)
    Q_PROPERTY(bool accept_control READ acceptControlChange WRITE setAcceptControlChange)
    Q_PROPERTY(bool accept_note READ acceptNoteChange WRITE setAcceptNoteChange)
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

    // Message filters
    bool acceptClock() { return _acceptClock; }
    bool acceptProgramChange() { return _acceptProgramChange; }
    bool acceptControlChange() { return _acceptControlChange; }
    bool acceptNoteChange() { return _acceptNoteChange; }

    // Used (when at least one message type is accepted)
    bool isUsed() { return (_acceptClock || _acceptProgramChange || _acceptControlChange || _acceptNoteChange); }

    // RtMidi callback
    // Warning: Should not be used by user...
    void midiCallback( double deltatime, std::vector< unsigned char > *message);
private:
    RtMidiIn *_rtMidiIn;
    int _id;
    unsigned int _portIndex;
    QString _portName;
    bool _connected;

    bool _acceptClock;
    bool _acceptProgramChange;
    bool _acceptControlChange;
    bool _acceptNoteChange;

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
    void setAcceptClock(bool on);
    void setAcceptProgramChange(bool on) { _acceptProgramChange = on; }
    void setAcceptControlChange(bool on) { _acceptControlChange = on; }
    void setAcceptNoteChange(bool on) { _acceptNoteChange = on; }

};

#endif // MIDIINTERFACE_H
