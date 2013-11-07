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

#ifndef MIDIINTERFACE_H
#define MIDIINTERFACE_H

#include <QObject>
#include <QSettings>

#include "RtMidi.h"

class Midi;

class MidiInterface : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ portName)
    Q_PROPERTY(QString mapping READ mapping WRITE setMapping)
public:
    explicit MidiInterface(const QString &portName, Midi *parent);
    ~MidiInterface();

    // Open port using _portName
    bool open();
    // Open port by name
    bool open(const QString& portName);

    // Close open MIDI connection (if one exists).
    bool close();

    // Test if MIDI is open
    bool isConnected();

    // Retrieve current connected port name
    QString portName() const;

    // ID
    int id() { return _id; }
    void setId(int id) { _id = id; }

    // Message filters
    bool acceptClock() { return _acceptClock; }
    bool acceptProgramChange() { return _acceptProgramChange; }
    bool acceptControlChange() { return _acceptControlChange; }
    bool acceptNoteChange() { return _acceptNoteChange; }

    // Used (when at least one message type is accepted)
    bool isUsed() { return !_mapping.isEmpty(); }

    // Mapping (Just keep a name (supposed to be unique) of corresponding mapping)
    QString mapping() const { return _mapping; }
    void loadMapping();
    void flushMapping();

    // Experimental: send a MIDI message
    bool sendMessage(const int channel, const int control, const int value);

    // RtMidi callback
    // Warning: Should not be used by user...
    void midiCallback( double deltatime, std::vector< unsigned char > *message);

private:
    Midi *_midi;
    RtMidiIn *_rtMidiIn;
    RtMidiOut *_rtMidiOut;
    int _id;
    unsigned int _portIndex;
    QString _portName;
    bool _connected;
    bool _hasOutput;
    QString _mapping;

    bool _acceptClock;
    bool _acceptProgramChange;
    bool _acceptControlChange;
    bool _acceptNoteChange;

    // Special accessor: we use objectName to store portName
    void setPortName(QString portName);

    // Open port by index (index is relative to current port list's index)
    bool openIn(const unsigned int index);
    bool openOut(const unsigned int portIndex);
    // Auto-(dis)connect function: ensure (dis)connection is made denpenging on isUsed()
    void autoconnect();

signals:
    void connected(bool connected = true);
    void clockReceived();
    void startReceived();
    void stopReceived();
    void continueReceived();
    void controlChanged(int interface, quint8 channel, quint8 control, quint8 value);
    void programChanged(int interface, quint8 channel, quint8 program);
    void noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value);
    void acceptClockChanged(bool clock);
    void acceptControlChanged(bool control);
    void acceptProgramChanged(bool program);
    void acceptNoteChanged(bool note);

public slots:
    void setAcceptClock(bool on);
    void setAcceptProgramChange(bool on);
    void setAcceptControlChange(bool on);
    void setAcceptNoteChange(bool on);
    void setMapping(const QString& mapping);
};

#endif // MIDIINTERFACE_H
