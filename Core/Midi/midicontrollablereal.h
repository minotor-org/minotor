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
