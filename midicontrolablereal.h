#ifndef MIDICONTROLABLEREAL_H
#define MIDICONTROLABLEREAL_H

#include "midicontrolableobject.h"

class MidiControlableReal : public MidiControlableObject
{
    Q_OBJECT
public:
    explicit MidiControlableReal(QObject *parent = 0);

    // value (0.0 - 1.0)
    qreal value() { return _value; }
    void setValue(qreal value);

protected:
    void setValueFromMidi(quint8 value);

private:
    void _setValue(qreal value);
    qreal _value;

signals:
    void valueChanged(qreal);

public slots:
    
};

#endif // MIDICONTROLABLEREAL_H
