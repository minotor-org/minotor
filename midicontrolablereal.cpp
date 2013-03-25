#include "midicontrolablereal.h"

MidiControlableReal::MidiControlableReal(QObject *parent) :
    MidiControlableObject(parent)
{
}

void MidiControlableReal::setValue(qreal value)
 {
    if(_value != value)
    {
        _value = value;
        emit valueChanged(value);
    }
    // qDebug() << Q_FUNC_INFO << "" << value;
 }

