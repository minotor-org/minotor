#include "midiinterface.h"

#include <QStringList>
#include <QDebug>

#include "midi.h"

MidiInterface::MidiInterface(QString portName, Midi *parent) :
    QObject(parent),
    _midi(parent),
    _rtMidiIn(NULL),
    _id(-1),
    _portIndex(0),
    _connected(false),
    _acceptClock(false),
    _acceptProgramChange(false),
    _acceptControlChange(false),
    _acceptNoteChange(false)
{
    setObjectName(portName);
    try
    {
        //Midi management
        _rtMidiIn = new RtMidiIn();
    } catch ( RtError &error ) {
        error.printMessage();
    }
}

MidiInterface::~MidiInterface()
{
    if(_rtMidiIn)
        delete _rtMidiIn;
}

/*
 * MIDI messages specs available at:
 *   http://www.midi.org/techspecs/midimessages.php
 */

// Channel voice messages (CVM) [nnnn = 0-15 (MIDI Channel Number 1-16)]
// These messages are channel-dependent: we may implement channel filter if needed...
#define MIDI_CVM_NOTE_OFF           0b10000000      // 1000nnnn
#define MIDI_CVM_NOTE_ON            0b10010000      // 1001nnnn
#define MIDI_CVM_CONTROL_CHANGE     0b10110000      // 1011nnnn
#define MIDI_CVM_PROGRAM_CHANGE     0b11000000      // 1100nnnn

// System Real-Time Messages (SRTM)
#define MIDI_SRTM_CLOCK      248  // 11111000
#define MIDI_SRTM_STOP       252  // 11111100
#define MIDI_SRTM_START      250  // 11111010
#define MIDI_SRTM_CONTINUE   251  // 11111011

void MidiInterface::midiCallback(double deltatime, std::vector< unsigned char > *message)
{
    (void)deltatime;
    unsigned char command = message->at(0);
    quint8 channel = command & 0x0f;
    if ((command&0xf0) != 0xf0) // if it is NOT a System message
    {
        command &= 0xf0; // It removes channel from MIDI message
    }

    switch(command) {
    case MIDI_CVM_NOTE_OFF:
        if(_acceptNoteChange)
            emit noteChanged(_id, quint8 (channel), quint8 (message->at(1)), false, quint8(message->at(2)));
        break;
    case MIDI_CVM_NOTE_ON:
        if(_acceptNoteChange)
            emit noteChanged(_id, quint8 (channel), quint8 (message->at(1)), true, quint8(message->at(2)));
        break;
    case MIDI_CVM_CONTROL_CHANGE:
        if(_acceptControlChange)
            emit controlChanged(_id, quint8 (channel), quint8 (message->at(1)), quint8(message->at(2)));
        break;
    case MIDI_CVM_PROGRAM_CHANGE:
        if(_acceptProgramChange)
            emit programChanged(_id, quint8 (channel), quint8 (message->at(1)));
        break;
    case MIDI_SRTM_CLOCK: emit clockReceived(); break;
    case MIDI_SRTM_STOP: emit stopReceived(); break;
    case MIDI_SRTM_START: emit startReceived(); break;
    case MIDI_SRTM_CONTINUE: emit continueReceived(); break;
    default:
        qDebug() << Q_FUNC_INFO
                 << QString("no handler for: %1 (%2)").arg(command).arg(command, 0, 16)
                 << "message size:" << message->size()
                 << "deltatime:" << deltatime;
    }
}

void _midiCallback(double deltatime, std::vector< unsigned char > *message, void *userData )
{
    (static_cast<MidiInterface*>(userData))->midiCallback(deltatime, message);
}

bool MidiInterface::open()
{
    // object name is used as portname
    return open(portName());
}

bool MidiInterface::open(QString portName)
{
    if(_rtMidiIn)
    {
        QStringList ports;
        // Check available ports.
        unsigned int nPorts = _rtMidiIn->getPortCount();
        for (unsigned int i=0;i<nPorts;i++)
        {
            ports.append(QString(_rtMidiIn->getPortName(i).c_str()));
        }
        int portIndex = ports.indexOf(portName);
        if(portIndex<0)
            return false;
        else
        {
            setPortName(portName);
            return open(portIndex);
        }
    }
    return false;
}

bool MidiInterface::open(const unsigned int portIndex)
{
    if(_rtMidiIn)
    {
        if(_connected)
        {
            qDebug() << Q_FUNC_INFO
                     << "Already opened port";
            return false;
        }
        else
        {
            _portIndex = portIndex;
            try {
                _rtMidiIn->openPort(portIndex);
                // Set our callback function.  This should be done immediately after
                // opening the port to avoid having incoming messages written to the
                // queue.
                _rtMidiIn->setCallback( &_midiCallback, this );

                // Don't ignore sysex, timing, or active sensing messages.
                _rtMidiIn->ignoreTypes( false, !_acceptClock, true );
                if(_id == -1)
                    setId(_midi->grabMidiInterfaceId());
                _connected = true;
                qDebug() << "MIDI connected to: " << this->portName();
                emit(connected());
            } catch ( RtError &error ) {
                error.printMessage();
                _connected = false;
            }
        }
    }
    return _connected;
}

bool MidiInterface::close()
{
    if(_rtMidiIn)
    {
        _rtMidiIn->closePort();
        _rtMidiIn->cancelCallback();
        _connected = false;
        qDebug() << "MIDI disconnected.";
        emit(connected(false));
    }
    return !_connected;
}

bool MidiInterface::isConnected()
{
    return _connected;
}

void MidiInterface::setPortName(QString portName)
{
    setObjectName(portName);
}

QString MidiInterface::portName()
{
    return objectName();
}

void MidiInterface::setAcceptClock(bool on)
{
    _acceptClock = on;
    if(_rtMidiIn)
    {
        // Don't ignore sysex, timing, or active sensing messages.
        _rtMidiIn->ignoreTypes( false, !_acceptClock, true );
    }
    isUsed() ? open() : close();
}

void MidiInterface::setAcceptProgramChange(bool on)
{
    _acceptProgramChange = on;
    isUsed() ? open() : close();
}

void MidiInterface::setAcceptControlChange(bool on)
{
    _acceptControlChange = on;
    isUsed() ? open() : close();
}

void MidiInterface::setAcceptNoteChange(bool on)
{
    _acceptNoteChange = on;
    isUsed() ? open() : close();
}
