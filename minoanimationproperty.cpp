#include "minoanimationproperty.h"

MinoAnimationProperty::MinoAnimationProperty(qreal value, QObject *parent) :
    QObject(parent),
    _value(value)
{
}

qreal MinoAnimationProperty::value()
{
    return _value;
}

void MinoAnimationProperty::midiControlValueChange(quint8 value)
{
    // MIDI control change's value range is 0 - 127 and _value is 0.0 - 1.0
    _value = ((qreal)value / 127);
    emit(valueChanged(_value));
}
