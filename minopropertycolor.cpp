#include "minopropertycolor.h"

#include <QDebug>

MinoPropertyColor::MinoPropertyColor(QObject *parent) :
    MinoProperty(parent),
    _color(1.0, 1.0, 1.0)
{
    _mcrHue = new MidiControllableReal(this);
    connect(_mcrHue, SIGNAL(valueChanged(qreal)), this, SLOT(setHue(qreal)));
    _mcrHue->setObjectName("hue");
    _mcrHue->setLabel("Hue");

    _mcrLightness = new MidiControllableReal(this);
    connect(_mcrLightness, SIGNAL(valueChanged(qreal)), this, SLOT(setLightness(qreal)));
    _mcrLightness->setObjectName("light");
    _mcrLightness->setLabel("Light");

    // Set default objectName and label
    setObjectName("color");
    setLabel("Color");
}

void MinoPropertyColor::setColor(const QColor& color)
{
    _mcrHue->setValue(color.hueF());
    _mcrLightness->setValue(color.lightnessF());
    _color = color;
}

QColor MinoPropertyColor::color()
{
    return _color;
}

void MinoPropertyColor::setHue(qreal value)
{
    _color.setHslF(value,1.0,_mcrLightness->value());
}

void MinoPropertyColor::setLightness(qreal value)
{
    _color.setHslF(_mcrHue->value(),1.0,value);
}
