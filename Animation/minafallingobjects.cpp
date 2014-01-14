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

#include "minafallingobjects.h"

#include <QDebug>

#include "minoanimationgroup.h"

MinaFallingObjects::MinaFallingObjects(QObject *object) :
    MinoInstrumentedAnimation(object)
{
    _ecrPosition.setStartValue(0.0);
    _ecrPosition.setEndValue(1.0);

    _beatDuration = new MinoPropertyBeat(this, true);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");

    _generatorDensity = new MinoPropertyReal(this);
    _generatorDensity->setObjectName("density");
    _generatorDensity->setLabel("Density");

    _generatorDirection = new MinoItemizedProperty(this);
    _generatorDirection->setObjectName("direction");
    _generatorDirection->setLabel("Direction");
    _generatorDirection->addItem("Right", 0);
    _generatorDirection->addItem("Left", 1);
    _generatorDirection->addItem("Up", 2);
    _generatorDirection->addItem("Down", 3);
    _generatorDirection->addItem("Rand.", 4);
    _generatorDirection->setCurrentItemFromString("Down");

    _generatorLength = new MinoPropertyReal(this);
    _generatorLength->setObjectName("length");
    _generatorLength->setLabel("Length");
    _generatorLength->setValue(0.4);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaFallingObjects::_createItem(const uint uppqn)
{
    createItem(uppqn, _color->color());
}

void MinaFallingObjects::createItem(const uint uppqn, const QColor& color)
{
    unsigned int direction = _generatorDirection->currentItem()->real();
    const unsigned int duration = _beatDuration->loopSizeInPpqn();
    const unsigned int length = qMax(1.0,(_generatorLength->value())*qMax(_boundingRect.width(),_boundingRect.height()));

    QGraphicsItem *item = NULL;

    int randomPos;
    unsigned int density = 0;
    if(direction == 4)
    {
        direction = qrand() % 4;
    }
    switch (direction)
    {
    case 0: // Left
    case 1: // Right
        density = 1 + (_generatorDensity->value() * (_boundingRect.height()-1));
        break;
    case 2: // Bottom
    case 3: // Top
        density = 1 + (_generatorDensity->value() * (_boundingRect.width()-1));
        break;
    }

    for (unsigned int i=0;i<density;i++)
    {
        const qreal progress = (qreal)i/(qreal)density;
        const qreal step = 1.0/(qreal)density;
        switch(direction)
        {
        case 0:
        {
            //left to right
            QLinearGradient grad(0.0, 0.0, length, 0.0);
            grad.setColorAt(0.0, Qt::transparent);
            grad.setColorAt(1, color);
            randomPos = (qreal)((progress + (qrandF()*step))*_boundingRect.height());
            item = _scene->addLine(0, randomPos, length, randomPos, QPen(QBrush(grad),1));
        }
            break;
        case 1:
        {
            //right to left
            QLinearGradient grad(0.0, 0.0, length, 0.0) ;
            grad.setColorAt(0.0, color) ;
            grad.setColorAt(1, Qt::transparent) ;
            randomPos = (qreal)((progress + (qrandF()*step))*_boundingRect.height());
            item = _scene->addLine(0, randomPos, length, randomPos, QPen(QBrush(grad),1));
        }
            break;
        case 2:
        {
            //bottom to top
            QLinearGradient grad(0.0, 0.0, 0.0, length) ;
            grad.setColorAt(0.0, color) ;
            grad.setColorAt(1, Qt::transparent) ;
            randomPos = (qreal)((progress + (qrandF()*step))*_boundingRect.width());
            item = _scene->addLine(randomPos, 0, randomPos, length, QPen(QBrush(grad),1));
        }
            break;
        case 3:
        {
            //top to bottom
            QLinearGradient grad(0.0, 0.0, 0.0, length) ;
            grad.setColorAt(0.0, Qt::transparent) ;
            grad.setColorAt(1, color) ;
            randomPos = (qreal)((progress + (qrandF()*step))*_boundingRect.width());
            item = _scene->addLine(randomPos, 0, randomPos, length, QPen(QBrush(grad),1));
        }
            break;
        }

        item->setData(MinaFallingObjects::Direction, direction);
        MinoAnimatedItem maItem (uppqn, duration, item);
        _itemGroup.addToGroup(item);
        _animatedItems.append(maItem);
    }
}

void MinaFallingObjects::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    _ecrPosition.setEasingCurve(_generatorCurve->easingCurveType());

    processNotesEvents(uppqn);
    processItemCreation(uppqn);

    if (_enabled && _beatFactor->isBeat(gppqn))
    {
        createItem(uppqn, _color->color());
    }
    const unsigned int length = qMax(1.0,(_generatorLength->value())*qMax(_boundingRect.width(),_boundingRect.height()));
    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        if (item.isCompleted(uppqn))
        {
            delete item._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal progress = item.progressForUppqn(uppqn);
            const int itemDirection = item.graphicsItem()->data(MinaFallingObjects::Direction).toInt();
            switch(itemDirection)
            {
            case 0:
            {
                //left to right
                _animatedItems.at(i)._graphicsItem->setPos((_ecrPosition.valueForProgress(progress)*((qreal)_boundingRect.width()+(length))-length),item._graphicsItem->pos().y());
            }
                break;
            case 1:
            {
                //right to left
                _animatedItems.at(i)._graphicsItem->setPos(((1-_ecrPosition.valueForProgress(progress))*((qreal)_boundingRect.width()+(length))-length),item._graphicsItem->pos().y());
                break;
            }
            case 2:
            {
                //bottom to top
                _animatedItems.at(i)._graphicsItem->setPos(item._graphicsItem->pos().x(),((1-_ecrPosition.valueForProgress(progress))*((qreal)_boundingRect.height()+(length))-length));
                break;
            }
            case 3:
            {
                //top to bottom
                _animatedItems.at(i)._graphicsItem->setPos(item._graphicsItem->pos().x(),((_ecrPosition.valueForProgress(progress))*((qreal)_boundingRect.height()+(length))-length));
                break;
            }
            }
        }
    }

    if(!_enabled && !_animatedItems.count())
    {
        MinoAnimation::setAlive(false);
        _alive = false;
    }
}

void MinaFallingObjects::_startNote(const uint uppqn, const quint8 note, const quint8 value)
{
    (void)value;
    createItem(uppqn, noteToColor(note));
}
