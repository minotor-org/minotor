#ifndef MIDICONTROLABLEREAL_H
#define MIDICONTROLABLEREAL_H

#include "midicontrolableobject.h"

class MidiControlableReal : public MidiControlableObject
{
    Q_OBJECT
public:
    explicit MidiControlableReal(QObject *parent = 0);

    // value (0.0 - 1.0)
    virtual qreal value() { return _value; }
    virtual void setValue(qreal value);


protected:
    void updateValueFromMidi(int value) { setValue((qreal)value/127); }

private:
    qreal _value;
    
signals:
    void valueChanged(qreal);

public slots:
    
};

#endif // MIDICONTROLABLEREAL_H
