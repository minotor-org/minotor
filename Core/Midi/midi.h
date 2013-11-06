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
    MidiInterface* interface(const QString& portName);
    MidiInterface* addMidiInterface(const QString& portName);
    MidiInterface* findMidiInterface(const int id);


public slots:
        // Scan interfaces
        void scanMidiInterfaces();

private:
    int grabMidiInterfaceId();
    void addMidiInterface(MidiInterface *interface);

    RtMidiIn *_midiIn;
    MidiInterfaces _interfaces;

signals:
    // Transport
    void clockReceived();
    void startReceived();
    void stopReceived();
    void continueReceived();

    // Control
    void controlChanged(int interface, quint8 channel, quint8 control, quint8 value);

    // Program
    void programChanged(int interface, quint8 channel, quint8 program);

    // Note
    void noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value);

    // CC, Note and program changes emit this signal
    void dataReceived();

    // One or more interface have been plugged/unplugged during scanMidiInterfaces()
    void updated();
};

#endif // MIDI_H
