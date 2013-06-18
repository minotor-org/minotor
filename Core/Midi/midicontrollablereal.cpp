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

#include "midicontrollablereal.h"

MidiControllableReal::MidiControllableReal(QObject *parent) :
    MidiControllableParameter(parent),
    _value(0)
{
}

void MidiControllableReal::setValue(qreal value)
{
    if(_value != value)
    {
        _value = value;
        MidiControllableParameter::setValueFromMidi(value*127);
        emit valueChanged(_value);
    }
}

void MidiControllableReal::setValueFromMidi(quint8 value)
{
     setValue((qreal)value/127.0);
     MidiControllableParameter::setValueFromMidi(value);
}
