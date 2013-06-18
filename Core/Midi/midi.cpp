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
    QObject(parent),
    _midiIn(NULL)
{
    try
    {
        //Midi management
        _midiIn = new RtMidiIn();
    } catch ( RtError &error ) {
        error.printMessage();
    }
    QStringList ports = getPorts();
    foreach(const QString& port, ports)
    {
        addMidiInterface(new MidiInterface(port, this));
    }
}

Midi::~Midi()
{
    if(_midiIn)
        delete _midiIn;
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

int Midi::grabMidiInterfaceId()
{
    bool freeIdFound = false;
    int id = 0;
    while(!freeIdFound)
    {
        bool idAlreadyExist = false;
        foreach(MidiInterface *mi, interfaces())
        {
            if(mi->id() == id)
            {
                idAlreadyExist = true;
                break;
            }
        }

        if(!idAlreadyExist)
        {
            freeIdFound = true;
            break;
        }
        id++;
    }
    return id;
}
