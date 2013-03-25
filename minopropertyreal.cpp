#include "minopropertyreal.h"

MinoPropertyReal::MinoPropertyReal(QObject *parent) :
    MinoMidiControlableProperty(parent),
    _value(0)
{
}

void MinoPropertyReal::setValue(qreal value)
{
    _midiValue = value*127.0;
    _setValue(value);
}

void MinoPropertyReal::setValueFromMidi(quint8 value)
{
     _setValue((qreal)value/127.0);
}

void MinoPropertyReal::_setValue(qreal value)
{
    if(_value != value)
    {
        _value = value;
        emit valueChanged(_value);
    }
}
