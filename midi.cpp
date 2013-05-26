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
    foreach(QString port, ports)
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
