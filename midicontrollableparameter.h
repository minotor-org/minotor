#ifndef MIDICONTROLLABLEPARAMETER_H
#define MIDICONTROLLABLEPARAMETER_H

#include <QObject>

class MidiControllableParameter : public QObject
{
    friend class MidiMapping;
    Q_OBJECT
public:
    enum Attribute {
        None = 0x0,
        MidiControled = 0x1,
        Preferred = 0x2
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    explicit MidiControllableParameter(QObject *parent = 0);

    void setPreferred(bool on = true);
    bool isPreferred() { return attributes().testFlag(MidiControllableParameter::Preferred); }
    bool isMidiControlled() { return attributes().testFlag(MidiControllableParameter::MidiControled); }
    quint8 valueFromMidi() { return _value; }

    // Attributes
    MidiControllableParameter::Attributes attributes() { return _attributes; }
    void setAttributes(MidiControllableParameter::Attributes attributes);

public slots:
    virtual void setValueFromMidi(quint8 value);

signals:
    void valueFromMidiChanged(quint8 value);
    void attributesChanged();

private:
    quint8 _value;
    MidiControllableParameter::Attributes _attributes;

public slots:
    
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MidiControllableParameter::Attributes)

#endif // MIDICONTROLLABLEPARAMETER_H
