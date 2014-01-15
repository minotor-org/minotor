/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    _mcrSaturation = new MidiControllableReal(this);
    connect(_mcrSaturation, SIGNAL(valueChanged(qreal)), this, SLOT(setSaturation(qreal)));
    _mcrSaturation->setObjectName("saturation");
    _mcrSaturation->setLabel("Saturation");

    // Set default objectName and label
    setObjectName("color");
    setLabel("Color");
}

void MinoPropertyColor::setColor(const QColor& color)
{
    _mcrHue->setValue(color.hueF());
    _mcrLightness->setValue(color.lightnessF());
    _mcrSaturation->setValue(color.saturationF());
    _color = color;
}

QColor MinoPropertyColor::color() const
{
    return _color;
}

void MinoPropertyColor::setHue(const qreal value)
{
    _color.setHslF(value,_mcrSaturation->value(),_mcrLightness->value());
}

void MinoPropertyColor::setLightness(const qreal value)
{
    _color.setHslF(_mcrHue->value(),_mcrSaturation->value(),value);
}

void MinoPropertyColor::setSaturation(const qreal value)
{
    _color.setHslF(_mcrHue->value(),value,_mcrLightness->value());
}
