#ifndef MINOANIMATIONPROPERTY_H
#define MINOANIMATIONPROPERTY_H

#include <QObject>

#include "midicontrol.h"

class MinoAnimationProperty : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimationProperty(qreal value = 0.0, QObject *parent = 0);

    qreal value() { return _value; }
    void setValue(qreal value) { _value = value; }
    void setMidiControl(MidiControl *control);
    MidiControl *midiControl() { return _midiControl; }
    bool isMidiControlled() { return _midiControl; }
    qreal step() { return _step; }
    void setStep(qreal step) { _step = step; }

signals:
    void valueChanged(qreal value);
public slots:
    void midiControlValueChange(quint8 value);
protected:
    qreal _value;
    qreal _step;
    MidiControl *_midiControl;
};

typedef QList<MinoAnimationProperty*> MinoAnimationPropertyList;

#endif // MINOANIMATIONPROPERTY_H
