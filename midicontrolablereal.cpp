#include "midicontrolablereal.h"

MidiControlableReal::MidiControlableReal(QObject *parent) :
    MidiControlableObject(parent),
    _value(0)
{
}

void MidiControlableReal::setValue(qreal value)
{
//    _midiValue = value*127.0;
    _setValue(value);
}

void MidiControlableReal::setValueFromMidi(quint8 value)
{
     _setValue((qreal)value/127.0);
}

void MidiControlableReal::_setValue(qreal value)
{
    if(_value != value)
    {
        _value = value;
        emit valueChanged(_value);
    }
}
