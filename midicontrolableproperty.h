#ifndef MIDICONTROLABLEPROPERTY_H
#define MIDICONTROLABLEPROPERTY_H

#include "midicontrolableobject.h"

class MinoMidiControlableProperty;

class MidiControlableProperty : public MidiControlableObject
{
    Q_OBJECT
public:
    explicit MidiControlableProperty(QObject *parent = 0);
    void setValueFromMidi(quint8 value);

private:
    MinoMidiControlableProperty *_property;
signals:
    
public slots:
    
};

#endif // MIDICONTROLABLEPROPERTY_H
