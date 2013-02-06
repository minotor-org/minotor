#include "midi.h"

#include <QDebug>

Midi::Midi(QObject *parent) :
    QObject(parent),
    _midiIn(NULL),
    _connected(false)
{
    try {
        //Midi management
        _midiIn = new RtMidiIn();
    } catch ( RtError &error ) {
        error.printMessage();
    }
}

QStringList Midi::getPorts()
{
    QStringList ports;
    if(_midiIn)
    {
        // Check available ports.
        unsigned int nPorts = _midiIn->getPortCount();
        for (unsigned int i=0;i<nPorts;i++)
        {
            ports.append(QString(_midiIn->getPortName(i).c_str()));
        }
    }
    return ports;
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

// System Real-Time Messages (SRTM)
#define MIDI_SRTM_CLOCK      248  // 11111000
#define MIDI_SRTM_STOP       252  // 11111100
#define MIDI_SRTM_START      250  // 11111010
#define MIDI_SRTM_CONTINUE   251  // 11111011

void Midi::midiCallback(double deltatime, std::vector< unsigned char > *message)
{
//    unsigned int nBytes = message->size();
    (void)deltatime;
    unsigned char command = message->at(0);
    quint8 channel = command & 0x0f;
    if ((command&0xf0) != 0xf0) // if it is NOT a System message
    {
        command &= 0xf0; // It removes channel from MIDI message
    }

    switch(command) {
    /* TODO implement me! */
    case MIDI_CVM_NOTE_OFF: /* qDebug() << "Note OFF:" << quint8 (message->at(1)); */
        emit noteChanged(quint8 (channel), false, quint8 (message->at(1)), quint8(message->at(2)));
        break;
    /* TODO implement me! */
    case MIDI_CVM_NOTE_ON:  /* qDebug() << "Note ON:" << quint8 (message->at(1)); */
        emit noteChanged(quint8 (channel), true, quint8 (message->at(1)), quint8(message->at(2)));
        break;
    case MIDI_CVM_CONTROL_CHANGE: emit controlChanged(quint8 (channel), quint8 (message->at(1)), quint8(message->at(2))); break;
    case MIDI_SRTM_CLOCK: emit clockReceived(); break;
    case MIDI_SRTM_STOP: emit stopReceived(); break;
    case MIDI_SRTM_START: emit startReceived(); break;
    case MIDI_SRTM_CONTINUE: emit continueReceived(); break;
    default:
        qDebug() << "no handler for" << command;
    }

    /*
    for ( unsigned int i=0; i<nBytes; i++ )
    {
        qDebug() << "Byte " << i << " = " << (int)message->at(i) << ", ";
        if ( nBytes > 0 )
          qDebug() << "stamp = " << deltatime;
    }
    */
}

void _midiCallback(double deltatime, std::vector< unsigned char > *message, void *userData )
{
    ((Midi*)userData)->midiCallback(deltatime, message);
}

bool Midi::openPort(const unsigned int portIndex)
{
    _portIndex = portIndex;
    try {
        _midiIn->openPort(portIndex);
        // Set our callback function.  This should be done immediately after
        // opening the port to avoid having incoming messages written to the
        // queue.
        _midiIn->setCallback( &_midiCallback, this );

        // Don't ignore sysex, timing, or active sensing messages.
        _midiIn->ignoreTypes( false, false, false );
         _connected = true;
        qDebug() << "MIDI connected to: " << this->portName();
        emit(connected());
    } catch ( RtError &error ) {
        error.printMessage();
        _connected = false;
    }
    return _connected;
}

void Midi::closePort()
{
    _midiIn->closePort();
    _midiIn->cancelCallback();
    _connected = false;
    qDebug() << "MIDI disconnected.";
    emit(connected(false));
}

bool Midi::isOpen()
{
    return _connected;
}

QString Midi::portName()
{
    if (_connected) {
        return QString(_midiIn->getPortName(_portIndex).c_str());
    }
    return "";
}

