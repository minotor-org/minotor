#include "midi.h"

#include <QDebug>

Midi::Midi(QObject *parent) :
    QObject(parent)
{
    //Midi management
    _midiIn = new RtMidiIn();
}

QStringList Midi::getPorts()
{
    QStringList ports;
    // Check available ports.
    unsigned int nPorts = _midiIn->getPortCount();
    for (int i=0;i<nPorts;i++)
    {
        ports.append(QString(_midiIn->getPortName(i).c_str()));
    }
    return ports;
}

#define MIDI_CLOCK      248
#define MIDI_STOP       252
#define MIDI_START      250
#define MIDI_CONTINUE   251
void Midi::midiCallback(double deltatime, std::vector< unsigned char > *message)
{
    unsigned int nBytes = message->size();

    const unsigned char command = message->at(0);
    switch(command) {
    case MIDI_CLOCK: emit clockReceived(); break;
    case MIDI_STOP: emit stopReceived(); break;
    case MIDI_START: emit startReceived(); break;
    case MIDI_CONTINUE: emit continueReceived(); break;
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

void Midi::openPort(const unsigned int portIndex)
{
    _midiIn->openPort(portIndex);

    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    _midiIn->setCallback( &_midiCallback, this );

    // Don't ignore sysex, timing, or active sensing messages.
    _midiIn->ignoreTypes( false, false, false );
}
