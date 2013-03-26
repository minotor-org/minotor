#include "minopropertycolor.h"

MinoPropertyColor::MinoPropertyColor(QObject *parent) :
    MinoProperty(parent)
{
    _mcrHue = new MidiControlableReal(this);
    _mcrLightness = new MidiControlableReal(this);
}

void MinoPropertyColor::setColor(QColor color)
{
    _mcrHue->setValue(color.hueF());
    _mcrLightness->setValue(color.lightnessF());
    _color = color;
}

QColor MinoPropertyColor::color()
{
    _color.setHslF(_mcrHue->value(), 1.0, _mcrLightness->value());
    return _color;
}
