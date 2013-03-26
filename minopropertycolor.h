#ifndef MINOPROPERTYCOLOR_H
#define MINOPROPERTYCOLOR_H

#include "minoproperty.h"
#include "midicontrolablereal.h"

#include <QColor>

class MinoPropertyColor : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoPropertyColor(QObject *parent = 0);
    
    void setColor(QColor color);
    QColor color();

private:
    MidiControlableReal *_mcrHue;
    MidiControlableReal *_mcrLightness;
    QColor _color;
signals:
    
public slots:
    
};

#endif // MINOPROPERTYCOLOR_H
