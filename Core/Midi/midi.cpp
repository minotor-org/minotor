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

#include "midi.h"

#include <QDebug>

#include "midiinterface.h"

Midi::Midi(QObject *parent) :
    QObject(parent)
{
    try
    {
        //Midi management
        MidiInterface *mi = new MidiInterface("Virtual MIDI In", this, MidiInterface::Virtual);
        if(mi->hasVirtualSupport())
        {
            mi->open();
            addMidiInterface(mi);
        }
    } catch ( RtMidiError &error ) {
        error.printMessage();
    }
    scanMidiInterfaces();
}

void Midi::scanMidiInterfaces()
{
    bool modified = false;

    QStringList midiInterfacePortNames;
    MidiInterfaces midiInterfaces = interfaces();

    Q_ASSERT(midiInterfaces.count());
    QStringList ports = midiInterfaces.at(0)->getPorts();

    foreach(MidiInterface *mi, midiInterfaces)
    {
        midiInterfacePortNames.append(mi->portName());
    }

    foreach(const QString& port, ports)
    {
        if(midiInterfacePortNames.contains(port))
        {
            const int id = midiInterfacePortNames.indexOf(port);

            // Attempt to reconnect interface
            MidiInterface *mi = midiInterfaces.at(id);
            if(mi->isUsed() && (!mi->isConnected()))
                modified |= midiInterfaces.at(id)->open();

            // Remove from interfacePortNames:
            //   It will only remains orphelin midi interface
            midiInterfacePortNames.removeAt(id);
            midiInterfaces.removeAt(id);
        } else {
            addMidiInterface(new MidiInterface(port, this));
            modified = true;
        }
    }

    // In midiInterfaces, only remains not-available-anymore ports (candidate to deletion)
    foreach(MidiInterface *mi, midiInterfaces)
    {
        if(!mi->isVirtual())
        {
            if(mi->isConnected())
                modified |= mi->close();
            if(mi->isUsed())
                continue;
            if(!ports.contains(mi->portName()))
            {
                delete mi;
                modified = true;
            }
        }
    }

    if (modified)
    {
        emit updated();
    }
}

Midi::~Midi()
{
}

QStringList Midi::getPorts(RtMidi *rtmidi)
{
    QStringList ports;
    if(rtmidi)
    {
        // Check available ports.
        unsigned int nPorts = rtmidi->getPortCount();
        for (unsigned int i=0;i<nPorts;i++)
        {
            ports.append(QString(rtmidi->getPortName(i).c_str()));
        }
    }
    return ports;
}

MidiInterface* Midi::addMidiInterface(const QString &portName)
{
    MidiInterface * mi = new MidiInterface(portName, this);
    addMidiInterface(mi);
    return mi;
}

MidiInterfaces Midi::interfaces()
{
    return findChildren<MidiInterface*>();
}

MidiInterface* Midi::interface(const QString &portName)
{
    return findChild<MidiInterface*>(portName);
}

void Midi::addMidiInterface(MidiInterface *interface)
{
    interfaces().append(interface);
    connect(interface, SIGNAL(clockReceived()), this, SIGNAL(clockReceived()));
    connect(interface, SIGNAL(startReceived()), this, SIGNAL(startReceived()));
    connect(interface, SIGNAL(stopReceived()), this, SIGNAL(stopReceived()));
    connect(interface, SIGNAL(continueReceived()), this, SIGNAL(continueReceived()));
    connect(interface, SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SIGNAL(controlChanged(int,quint8,quint8,quint8)));
    connect(interface, SIGNAL(programChanged(int,quint8,quint8)), this, SIGNAL(programChanged(int,quint8,quint8)));
    connect(interface, SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)), this, SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)));


    connect(interface, SIGNAL(startReceived()), this, SIGNAL(dataReceived()));
    connect(interface, SIGNAL(stopReceived()), this, SIGNAL(dataReceived()));
    connect(interface, SIGNAL(continueReceived()), this, SIGNAL(dataReceived()));
    connect(interface, SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SIGNAL(dataReceived()));
    connect(interface, SIGNAL(programChanged(int,quint8,quint8)), this, SIGNAL(dataReceived()));
    connect(interface, SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)), this, SIGNAL(dataReceived()));
}

MidiInterface *Midi::findMidiInterface(const int id)
{
    foreach(MidiInterface *mi, interfaces())
    {
        if(mi->id() == id)
        {
            return mi;
        }
    }
    return NULL;
}

int Midi::grabMidiInterfaceId()
{
    bool freeIdFound = false;
    int id = 0;
    while(!freeIdFound)
    {
        MidiInterface * mi = findMidiInterface(id);

        if(!mi)
        {
            freeIdFound = true;
            break;
        }
        id++;
    }
    return id;
}
