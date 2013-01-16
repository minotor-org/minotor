#include "midicontrol.h"

MidiControl::MidiControl(int interface, quint8 channel, quint8 control, QObject *parent) :
    QObject(parent),
    _interface(interface),
    _channel(channel),
    _control(control)
{
}

void MidiControl::setValue(quint8 value)
{
    _value = value;
    emit(valueChanged(_value));
}

int MidiControl::interface()
{
    return _interface;
}

quint8 MidiControl::channel()
{
    return _channel;
}

quint8 MidiControl::control()
{
    return _control;
}