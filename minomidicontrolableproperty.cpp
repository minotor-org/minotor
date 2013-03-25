#include "minomidicontrolableproperty.h"

#include "midicontrolableproperty.h"

#include <QDebug>

MinoMidiControlableProperty::MinoMidiControlableProperty(QObject *parent) :
    MinoProperty(parent),
    _midiValue(0),
    _step(0.0),
    _type(MinoMidiControlableProperty::Linear)
{
    _midiControlableProperty = new MidiControlableProperty(this);
    connect(_midiControlableProperty, SIGNAL(attributesChanged()), this, SIGNAL(attributesChanged()));
}

void MinoMidiControlableProperty::setPreferred(bool on)
{
    if (on)
    {
        _midiControlableProperty->setAttributes(_midiControlableProperty->attributes() | MidiControlableObject::Preferred);
    }
    else
    {
        _midiControlableProperty->setAttributes(_midiControlableProperty->attributes() & ~MidiControlableObject::Attributes(MidiControlableObject::Preferred));
    }
}

void MinoMidiControlableProperty::_setValueFromMidi(quint8 value)
{
    if(value != _midiValue)
    {
        _midiValue = value;
        setValueFromMidi(_midiValue);
        emit midiValueChanged(_midiValue);
    }
}

void MinoMidiControlableProperty::setMidiValue(quint8 midiValue)
{
    qDebug() << Q_FUNC_INFO;
    _setValueFromMidi(midiValue);
}
