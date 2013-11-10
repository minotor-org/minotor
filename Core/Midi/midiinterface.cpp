/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "midiinterface.h"

#include <QStringList>
#include <QDebug>

#include "midi.h"
#include "midimapping.h"
#include "midimapper.h"

#include "minotor.h"

#include <QRegExp>

MidiInterface::MidiInterface(const QString& portName, Midi *parent) :
    QObject(parent),
    _midi(parent),
    _rtMidiIn(NULL),
    _rtMidiOut(NULL),
    _id(-1),
    _portIndex(0),
    _connected(false),
    _hasOutput(false),
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
        _rtMidiOut = new RtMidiOut();
    } catch ( RtError &error ) {
        error.printMessage();
    }
}

MidiInterface::~MidiInterface()
{
    if(_rtMidiIn)
        delete _rtMidiIn;
    if(_rtMidiOut)
        delete _rtMidiOut;
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

// System Common Messages (SCM)
#define MIDI_SCM_SYSEX   240  // 11110000

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
    case MIDI_SCM_SYSEX:
    {
        // HACK to use Korg nanoKontrol scene button in order to change master view
        if(message->size() == 11)
        {
            emit programChanged(_id, quint8 (channel), quint8 (message->at(9)));
        }
    }
        break;
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

bool MidiInterface::open(const QString& portName)
{
    if(_rtMidiIn)
    {
        QStringList ports;
        // Check available ports.
        unsigned int nPorts = _rtMidiIn->getPortCount();
        for (unsigned int i=0;i<nPorts;i++)
        {
            const QString inputName = QString(_rtMidiIn->getPortName(i).c_str());
            ports.append(inputName);
//            qDebug() << "In port: " << inputName;
        }
        int portIndex = ports.indexOf(portName);
        if(portIndex<0)
            return false;
        else
        {
            setPortName(portName);
            if (!openIn(portIndex)) { return false; }
        }
        if(_rtMidiOut)
        {
            QStringList ports;
            // Check available ports.
            unsigned int nPorts = _rtMidiOut->getPortCount();
            for (unsigned int i=0;i<nPorts;i++)
            {
                ports.append(QString(_rtMidiOut->getPortName(i).c_str()));
//                qDebug() << "Out port: " << QString(_rtMidiOut->getPortName(i).c_str());
            }
            // "nanoKONTROL2 28:0"  -> "nanoKONTROL2:0"
            QRegExp rx("(\\w+) (.+):(\\d+)");
            if(rx.indexIn(portName) == -1)
            {
                qDebug() << Q_FUNC_INFO
                         << "no match for:" << portName;
                Q_ASSERT(false);
            }
            else
            {
                QStringList sl = rx.capturedTexts();
//                    qDebug() << Q_FUNC_INFO
//                             <<   sl;
                Q_ASSERT(sl.count()==4);
                const QString outputName = sl.at(1) + QString(":") + sl.at(3);
                int portIndex = ports.indexOf(outputName);
                if(portIndex>=0)
                {
                    openOut(portIndex);
                }
            }
        }
        // Auto load mapping
        loadMapping();
        return true;
    }
    return false;
}

bool MidiInterface::openIn(const unsigned int portIndex)
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
                qDebug() << "MIDI In connected to: " << this->portName();
                emit(connected());
            } catch ( RtError &error ) {
                error.printMessage();
                _connected = false;
            }
        }
    }
    return _connected;
}

bool MidiInterface::openOut(const unsigned int portIndex)
{
    if(_rtMidiOut)
    {
        if(_hasOutput)
        {
            qDebug() << Q_FUNC_INFO
                     << "Already opened port";
            return false;
        }
        else
        {
            _portIndex = portIndex;
            try {
                _rtMidiOut->openPort(portIndex);
                _hasOutput = true;
                qDebug() << "MIDI Out connected to: " << this->portName();
            } catch ( RtError &error ) {
                error.printMessage();
                _hasOutput = false;
            }
        }
    }
    return _hasOutput;
}

bool MidiInterface::sendMessage(const int channel, const int control, const int value)
{
    if (_hasOutput)
    {
        std::vector< unsigned char > message;
        message.push_back(MIDI_CVM_CONTROL_CHANGE | (channel&0x0f));
        message.push_back(control);
        message.push_back(value);
        _rtMidiOut->sendMessage(&message);
    }
    return true;
}

bool MidiInterface::sendMessage(const QByteArray &bytes)
{
    if (_hasOutput)
    {
        std::vector< unsigned char > message;
        for(int i=0; i<bytes.count(); ++i)
        {
            message.push_back(bytes[i]);
        }
        _rtMidiOut->sendMessage(&message);
        qDebug() << Q_FUNC_INFO << "data:" << bytes.toHex();
    }
    return true;
}

bool MidiInterface::close()
{
    if(_rtMidiIn && _connected)
    {
        _rtMidiIn->closePort();
        _rtMidiIn->cancelCallback();
        _connected = false;
        if(_rtMidiOut && _hasOutput)
        {
            _rtMidiOut->closePort();
            _hasOutput = false;
        }
        qDebug() << "MIDI disconnected.";
        emit(connected(false));
        flushMapping();
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

QString MidiInterface::portName() const
{
    return objectName();
}

void MidiInterface::autoconnect()
{
    if(isUsed())
    {
        if(!_connected)
            open();
    }
    else
    {
        if(_connected)
            close();
    }
}

void MidiInterface::setAcceptClock(bool on)
{
    _acceptClock = on;
    if(_rtMidiIn)
    {
        // Don't ignore sysex, timing, or active sensing messages.
        _rtMidiIn->ignoreTypes( false, !_acceptClock, true );
    }
    autoconnect();
}

void MidiInterface::setAcceptProgramChange(bool on)
{
    _acceptProgramChange = on;
    autoconnect();
}

void MidiInterface::setAcceptControlChange(bool on)
{
    _acceptControlChange = on;
    autoconnect();
}

void MidiInterface::setAcceptNoteChange(bool on)
{
    _acceptNoteChange = on;
    autoconnect();
}

void MidiInterface::setMapping(const QString& mapping)
{
    if(!_mapping.isEmpty())
    {
        if(_connected) flushMapping();
    }
    _mapping = mapping;

    if(_mapping.isEmpty())
    {
        setAcceptClock(false);
        setAcceptProgramChange(false);
        setAcceptControlChange(false);
        setAcceptNoteChange(false);
        close();
    }
    else
    {
        if(_connected) loadMapping();
    }
}

void MidiInterface::loadMapping()
{
    if(!_mapping.isEmpty())
    {
        MidiMapping * mm = MidiMapping::loadFromFile(_mapping);
        if(mm)
        {
            Minotor::minotor()->midiMapper()->loadMidiMapping(this, mm);
            delete mm;
        } else {
            qDebug() << Q_FUNC_INFO
                     << "Invalid file" << _mapping;
            // Do keep invalid mapping filename
            setMapping("");
        }
    }
}

void MidiInterface::flushMapping()
{
    Minotor::minotor()->midiMapper()->flushMidiMapping(this);
}
