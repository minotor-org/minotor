#ifndef MINOPROPERTYREAL_H
#define MINOPROPERTYREAL_H

#include "minoproperty.h"
#include "minomidicontrolableproperty.h"

class MinoPropertyReal : public MinoMidiControlableProperty
{
    Q_OBJECT
public:
    explicit MinoPropertyReal(QObject *parent = 0);
    
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

#endif // MINOPROPERTYREAL_H
