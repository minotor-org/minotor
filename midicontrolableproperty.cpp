#include "midicontrolableproperty.h"

#include "minomidicontrolableproperty.h"

MidiControlableProperty::MidiControlableProperty(QObject *parent) :
    MidiControlableObject(parent),
    _property(NULL)
{
    MinoMidiControlableProperty * property = dynamic_cast<MinoMidiControlableProperty*>(parent);
    if (property)
    {
        _property = property;
    }
}

void MidiControlableProperty::setValueFromMidi(quint8 value)
{
     if(_property)
     {
         _property->_setValueFromMidi(value);
     }
}
