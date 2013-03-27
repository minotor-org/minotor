#include "midicontrolableobject.h"

MidiControlableObject::MidiControlableObject(QObject *parent) :
    MinoPersistentObject(parent),
    _midiControl(NULL),
    _attributes(0)
{
}

void MidiControlableObject::setMidiControl(MidiControl *control)
{
    if(_midiControl)
    {
        // Only one control allowed to change this property
        _midiControl->disconnect(this);
    }
    _midiControl = control;
    connect(_midiControl, SIGNAL(valueChanged(quint8)), this, SLOT(midiControlValueChange(quint8)));
    setAttributes(_attributes | MidiControlableObject::MidiControled);
}


void MidiControlableObject::setAttributes(MidiControlableObject::Attributes attributes)
{
    if(_attributes != attributes)
    {
        _attributes = attributes;
        emit attributesChanged();
    }
}

void MidiControlableObject::midiControlValueChange(quint8 value)
{
    setValueFromMidi(value);
}
