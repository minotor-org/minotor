#ifndef MINOMIDICONTROLABLEPROPERTY_H
#define MINOMIDICONTROLABLEPROPERTY_H

#include "minoproperty.h"
#include "midicontrolableproperty.h"

class MinoMidiControlableProperty : public MinoProperty
{
    Q_OBJECT
    friend class MidiControlableProperty;
    friend class MidiMapping;

public:
    explicit MinoMidiControlableProperty(QObject *parent = 0);
    
    void setPreferred(bool on = true);
    bool isPreferred() { return _midiControlableProperty->attributes().testFlag(MidiControlableObject::Preferred); }
    bool isMidiControlled() { return _midiControlableProperty->attributes().testFlag(MidiControlableObject::MidiControled); }
    quint8 midiValue() { return _midiValue; }
    void setMidiValue(quint8 midiValue);

    // Type
    enum Type { Linear, Steps, Items };
    MinoMidiControlableProperty::Type type() { return _type; }
    void setType(MinoMidiControlableProperty::Type type) { _type = type; }

    // step (0.0 - 1.0)
    virtual qreal step() { return _step; }
    virtual void setStep(qreal step) { _step = step; }

protected:
    MidiControlableProperty *_midiControlableProperty;
    quint8 _midiValue;
    virtual void setValueFromMidi(quint8 value) = 0;

private:
    qreal _step;

    MinoMidiControlableProperty::Type _type;

signals:
    void midiValueChanged(quint8);
    void attributesChanged();
    
public slots:
    // Directly called by MidiControlableProperty
    void _setValueFromMidi(quint8 value);

};

#endif // MINOMIDICONTROLABLEPROPERTY_H
