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

#include "minoanimation.h"

#include "minotor.h"
#include "minoprogram.h"
#include "minoanimationgroup.h"

MinoAnimation::MinoAnimation(QObject *parent) :
    MinoPersistentObject(parent),
    _group(NULL),
    _enabled(false),
    _currentRandY(0)
{
    Q_ASSERT(parent);
    if(MinoAnimationGroup* mag = qobject_cast<MinoAnimationGroup*>(parent))
    {
        setGroup(mag);
        _scene = Minotor::minotor()->scene();
        _boundingRect = Minotor::minotor()->displayRect();
    }
    _color = new MinoPropertyColor(this);
    QColor randColor;
    randColor.setHslF(qrandF(), 1.0, 0.5);
    _color->setColor(randColor);

    _beatFactor = new MinoPropertyBeat(this);
}

int MinoAnimation::id()
{
    return _group->animations().indexOf(this);
}

QPointF MinoAnimation::qrandPointF()
{
    return QPointF(qrandF()*_boundingRect.width(), qrandF()*_boundingRect.height());
}

void MinoAnimation::setGroup(MinoAnimationGroup *group)
{
    if(_group != group)
    {
        if(_group)
        {
            disconnect(this, SIGNAL(destroyed(QObject*)), _group, SLOT(destroyAnimation(QObject*)));
        }
        setParent(group);
        if(group) {
           connect(this, SIGNAL(destroyed(QObject*)), group, SLOT(destroyAnimation(QObject*)));
        } else {
            graphicItem()->setVisible(false);
        }
        _group = group;
    }
}

void MinoAnimation::setEnabled(const bool on)
{
    if(on != _enabled)
    {
        _enabled = on;
        setAlive(on);
        emit enabledChanged(on);
    }
}

unsigned int MinoAnimation::qrandY(unsigned int size)
{
    qreal maxRand = qreal(Minotor::minotor()->rendererSize().height())-qreal(size)-1.0;
    if (maxRand>0.0)
    {
        unsigned int rand = qreal(qrandF() * maxRand);
        _currentRandY = (_currentRandY+rand+1)%((Minotor::minotor()->rendererSize().height()-size));
    }
    return _currentRandY;
}
