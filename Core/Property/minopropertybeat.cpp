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

#include "minopropertybeat.h"

#include <QDebug>

MinoPropertyBeat::MinoPropertyBeat(QObject *parent, bool reversedOrder) :
    MinoProperty(parent)
{
    _mcl = new MidiControllableList(this);
    setObjectName("loop-size");
    _mcl->setLabel("Loop");
    _mcl->setLinear();

    if (reversedOrder) {
        _mcl->addItem("1/4", 6);
        _mcl->addItem("1/2", 12);
        _mcl->addItem("1", 24);
        _mcl->addItem("2", 48);
        _mcl->addItem("4", 96);
        _mcl->addItem("8", 192);
        _mcl->addItem("16", 384);
    }
    else
    {
        _mcl->addItem("16", 384);
        _mcl->addItem("8", 192);
        _mcl->addItem("4", 96);
        _mcl->addItem("2", 48);
        _mcl->addItem("1", 24);
        _mcl->addItem("1/2", 12);
        _mcl->addItem("1/4", 6);
    }
    _mcl->setCurrentItemFromString("1");

    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(loopSizeChanged(QString)));
}

qreal MinoPropertyBeat::progressForGppqn(const unsigned int gppqn) const
{
    const unsigned int ppqnMax = loopSizeInPpqn();
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    return durationFactor;
}

void MinoPropertyBeat::setObjectName(const QString &name)
{
    _mcl->setObjectName(name);
    MinoProperty::setObjectName(name);
}

bool MinoPropertyBeat::isBeat(const unsigned int gppqn) const
{
    const unsigned int loopSize = loopSizeInPpqn();
    return(gppqn%loopSize)==0;
}
