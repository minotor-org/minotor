#ifndef MIDICONTROLABLEOBJECT_H
#define MIDICONTROLABLEOBJECT_H

#include "minopersistentobject.h"
#include "midicontrol.h"

class MidiControlableObject : public MinoPersistentObject
{
    Q_OBJECT
public:
    enum Attribute {
        None = 0x0,
        MidiControled = 0x1,
        Preferred = 0x2
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    explicit MidiControlableObject(QObject *parent);

    // Pure virtual callback
    virtual void setValueFromMidi(quint8 value) = 0;

    // MIDI
    void setMidiControl(MidiControl *midiControl);
    MidiControl *midiControl() { return _midiControl; }
    bool isMidiControlled() { return _midiControl; }

    // Attributes
    MidiControlableObject::Attributes attributes() { return _attributes; }
    void setAttributes(MidiControlableObject::Attributes attributes);

private:
    MidiControl *_midiControl;
    MidiControlableObject::Attributes _attributes;

signals:
    void attributesChanged();

public slots:
    void midiControlValueChange(quint8);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(MidiControlableObject::Attributes)

#endif // MIDICONTROLABLEOBJECT_H
