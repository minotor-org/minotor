#ifndef MINOPROPERTYCOLOR_H
#define MINOPROPERTYCOLOR_H

#include "minoproperty.h"
#include "midicontrollablereal.h"

#include <QColor>

class MinoPropertyColor : public MinoProperty
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor STORED true)

public:
    explicit MinoPropertyColor(QObject *parent);
    
    void setColor(const QColor &color);
    QColor color() const;

private:
    MidiControllableReal *_mcrHue;
    MidiControllableReal *_mcrLightness;
    QColor _color;
signals:
    
public slots:
    void setHue(qreal value);
    void setLightness(qreal value);

};

#endif // MINOPROPERTYCOLOR_H