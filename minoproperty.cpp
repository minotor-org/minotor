#include "minoproperty.h"

#include <QDebug>

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
    setValue((qreal)value / 127);
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
    setAttributes(_attributes | MinoProperty::MidiControled);
}

void MinoProperty::setValue(qreal value)
 {
    if(_value != value)
    {
        _value = value;
        emit valueChanged(value);
    }
    qDebug() << Q_FUNC_INFO
             << "" << value;
 }

MinoProperty::Type MinoProperty::type()
{
    return MinoProperty::Linear;
}

void MinoProperty::setAttributes(MinoProperty::Attributes attributes)
{
    if(_attributes != attributes)
    {
        _attributes = attributes;
        emit attributesChanged();
    }
}
