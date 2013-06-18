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

#ifndef MINOPROPERTYREAL_H
#define MINOPROPERTYREAL_H

#include "minoproperty.h"
#include "midicontrollablereal.h"

class MinoPropertyReal : public MinoProperty
{
    Q_OBJECT
    Q_PROPERTY(qreal value READ value WRITE setValue STORED true)
public:
    explicit MinoPropertyReal(QObject *parent);

    // Label: Propagate label name to _mcr
    void setLabel(const QString &label);
    void setObjectName(const QString &name);

    // value (0.0 - 1.0)
    qreal value() { return _mcr->value(); }
    void setValue(qreal value) { return _mcr->setValue(value); }
    void setPreferred(bool on = true) { _mcr->setPreferred(on); }

protected slots:

private:
    MidiControllableReal *_mcr;

signals:
    void valueChanged(qreal);

public slots:
    
};

#endif // MINOPROPERTYREAL_H
