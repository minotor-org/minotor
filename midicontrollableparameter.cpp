#include "midicontrollableparameter.h"

MidiControllableParameter::MidiControllableParameter(QObject *parent) :
    QObject(parent),
    _value(0)
{
}

void MidiControllableParameter::setPreferred(bool on)
{
    if (on)
    {
        setAttributes(attributes() | MidiControllableParameter::Preferred);
    }
    else
    {
        setAttributes(attributes() & ~MidiControllableParameter::Attributes(MidiControllableParameter::Preferred));
    }
}

void MidiControllableParameter::setAttributes(MidiControllableParameter::Attributes attributes)
{
    if(_attributes != attributes)
    {
        _attributes = attributes;
        emit attributesChanged();
    }
}

void MidiControllableParameter::setValueFromMidi(quint8 value)
{
    if(value != _value)
    {
        _value = value;
        emit valueFromMidiChanged(value);
    }
}
