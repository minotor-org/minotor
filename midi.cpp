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

MidiInterfaces Midi::interfaces()
{
    return findChildren<MidiInterface*>();
}

MidiInterface* Midi::interface(QString portName)
{
    /*
    foreach(MidiInterface * midiInterface, findChildren<MidiInterface*>())
    {
        qDebug() << Q_FUNC_INFO
                    << midiInterface << midiInterface->objectName();
    }
    */

    return findChild<MidiInterface*>(portName);
}

void Midi::addMidiInterface(MidiInterface *interface)
{
    interface->_id = interfaces().count();
    interfaces().append(interface);
    connect(interface, SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SIGNAL(controlChanged(int,quint8,quint8,quint8)));
    connect(interface, SIGNAL(programChanged(int,quint8,quint8)), this, SIGNAL(programChanged(int,quint8,quint8)));
    connect(interface, SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)), this, SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)));
    /*
    qDebug() << Q_FUNC_INFO
             << "Midi interface added:" << interface->objectName();
    */
}
