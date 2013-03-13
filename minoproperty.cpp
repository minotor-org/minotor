#include "minoproperty.h"

MinoProperty::MinoProperty(qreal value, QObject *parent) :
    QObject(parent),
    _value(value),
    _step(0.0),
    _midiControl(NULL)
{
}

void MinoProperty::midiControlValueChange(quint8 value)
{
    // MIDI control change's value range is 0 - 127 and _value is 0.0 - 1.0
    _value = ((qreal)value / 127);
    emit(valueChanged(_value));
}

void MinoProperty::setMidiControl(MidiControl *control)
{
    if(_midiControl)
    {
        // Only one control allowed to change this property
        _midiControl->disconnect(this);
    }
    _midiControl = control;
    connect(_midiControl, SIGNAL(valueChanged(quint8)), this, SLOT(midiControlValueChange(quint8)));
}

void MinoProperty::setValue(qreal value)
 {
    if(_value != value)
    {
        _value = value;
        emit valueChanged(value);
    }
 }

MinoProperty::Type MinoProperty::type()
{
    return MinoProperty::Linear;
}
