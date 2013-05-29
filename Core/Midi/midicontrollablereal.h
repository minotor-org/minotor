#ifndef MIDICONTROLLABLEREAL_H
#define MIDICONTROLLABLEREAL_H

#include "midicontrollableparameter.h"

class MidiControllableReal : public MidiControllableParameter
{
    Q_OBJECT
public:
    explicit MidiControllableReal(QObject *parent = 0);

    // value (0.0 - 1.0)
    qreal value() { return _value; }
    void setValue(qreal value);

protected:
    void setValueFromMidi(quint8 value);

private:
    qreal _value;

signals:
    void valueChanged(qreal);

public slots:
    
};

#endif // MIDICONTROLLABLEREAL_H
