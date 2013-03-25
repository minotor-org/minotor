#include "minopropertyreal.h"

MinoPropertyReal::MinoPropertyReal(QObject *parent) :
    MinoMidiControlableProperty(parent)
{
}

void MinoPropertyReal::setValue(qreal value)
{
    if(_value != value)
    {
        _value = value;
        emit valueChanged(_value);
    }
}

void MinoPropertyReal::setValueFromMidi(quint8 value)
{
     setValue((qreal)value/127);
}
