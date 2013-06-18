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

#include "minoitemizedproperty.h"

#include <QDebug>

MinoItemizedProperty::MinoItemizedProperty(QObject *parent):
    MinoProperty(parent)
{
    _mcl = new MidiControllableList(this);
    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(itemChanged(QString)));
    connect(_mcl, SIGNAL(itemIdChanged(int)), this, SIGNAL(itemIdChanged(int)));
}

void MinoItemizedProperty::setObjectName(const QString &name)
{
    _mcl->setObjectName(name);
    MinoProperty::setObjectName(name);
}

void MinoItemizedProperty::addItem(const QString name, const qreal real)
{
    _mcl->addItem(name, real);
}

MidiControllableListItem *MinoItemizedProperty::currentItem()
{
    return _mcl->currentItem();
}

void MinoItemizedProperty::setCurrentItemIndex(int index)
{
    _mcl->setCurrentItemIndex(index);
}

void MinoItemizedProperty::setCurrentItem(const QString name)
{
    _mcl->setCurrentItem(name);
}

void MinoItemizedProperty::setLinear(bool linear)
{
    _mcl->setLinear(linear);
}

void MinoItemizedProperty::setLabel(const QString &label)
{
    _mcl->setLabel(label);
    MinoProperty::setLabel(label);
}
