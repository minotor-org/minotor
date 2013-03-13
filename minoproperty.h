#ifndef MINOPROPERTY_H
#define MINOPROPERTY_H

#include <QObject>
#include <QVariant>

#include "midicontrol.h"

class MinoProperty : public QObject
{
    Q_OBJECT
public:
    enum Attribute {
        MidiControled = 0x0,   // 0x000000
        Important = 0x1        // 0x000001
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    explicit MinoProperty(qreal value = 0.0, QObject *parent = 0);

    // value (0.0 - 1.0)
    virtual qreal value() { return _value; }
    virtual void setValue(qreal value);

    // step (0.0 - 1.0)
    virtual qreal step() { return _step; }
    virtual void setStep(qreal step) { _step = step; }

    // MIDI
    void setMidiControl(MidiControl *control);
    MidiControl *midiControl() { return _midiControl; }
    bool isMidiControlled() { return _midiControl; }

    MinoProperty::Attributes attributes() { return _attributes; }
    void setAttributes(MinoProperty::Attributes attributes) { _attributes = attributes; }

    enum Type { Linear, Steps, Items };
    virtual MinoProperty::Type type();

signals:
    void valueChanged(qreal value);
public slots:
    void midiControlValueChange(quint8 value);
protected:
    qreal _value;
    qreal _step;
    MidiControl *_midiControl;

    MinoProperty::Attributes _attributes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MinoProperty::Attributes)

typedef QList<MinoProperty*> MinoPropertyList;
typedef QList<MinoPropertyList*> MinoPropertyGrouped;
#endif // MINOPROPERTY_H
