#ifndef MINOPROPERTY_H
#define MINOPROPERTY_H

#include <QObject>
#include <QVariant>

#include "midicontrol.h"

class MinoProperty : public QObject
{
    Q_OBJECT
public:
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

signals:
    void valueChanged(qreal value);
public slots:
    void midiControlValueChange(quint8 value);
protected:
    qreal _value;
    qreal _step;
    MidiControl *_midiControl;

};

typedef QList<MinoProperty*> MinoPropertyList;

#endif // MINOPROPERTY_H
