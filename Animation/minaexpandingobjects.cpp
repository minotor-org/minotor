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

#include "minaexpandingobjects.h"

#include <QGraphicsBlurEffect>

#include <QDebug>

MinaExpandingObjects::MinaExpandingObjects(QObject *object):
    MinoInstrumentedAnimation(object)
{
    _ecrScale.setStartValue(0.01);
    _ecrScale.setEndValue(2.0);
    _ecrScale.setEasingCurve(QEasingCurve::Linear);

    _beatDuration = new MinoPropertyBeat(this, true);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");

    _generatorStyle = new MinoItemizedProperty(this);
    _generatorStyle->setObjectName("style");
    _generatorStyle->setLabel("Style");
    _generatorStyle->addItem("P:F T:F", 0);
    _generatorStyle->addItem("P:R T:F", 1);
    _generatorStyle->addItem("P:F T:R", 2);
    _generatorStyle->addItem("P:R=T:R", 3);
    _generatorStyle->addItem("P:R T:R", 4);
    _generatorStyle->setCurrentItemFromString("P:R T:F");

    _generatorShape = new MinoItemizedProperty(this);
    _generatorShape->setObjectName("shape");
    _generatorShape->setLabel("Shape");
    _generatorShape->addItem("Ellipse", 0);
    _generatorShape->addItem("Rect", 1);
    _generatorShape->addItem("Circle", 2);
    _generatorShape->addItem("Square", 3);
    _generatorShape->setCurrentItemFromString("Ellipse");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaExpandingObjects::_createItem(const uint uppqn)
{
    createItem(uppqn, _color->color());
}

void MinaExpandingObjects::createItem(const unsigned int uppqn, const QColor &color)
{
    QGraphicsItem* item = NULL;
    const unsigned int shape = _generatorShape->currentItem()->real();
    switch(shape)
    {
    case 0:
        // HACK ellipse draw is 1 pixel larger than needed
        item = _scene->addEllipse(_boundingRect.adjusted(0,0,-1,-1), QPen(color), QBrush(Qt::NoBrush));
        break;
    case 1:
    {
        // HACK rect draw is 1 pixel larger than needed
        item = _scene->addRect(_boundingRect.adjusted(0,0,-1,-1), QPen(color), QBrush(Qt::NoBrush));
    }
        break;
    case 2:
    {
        const qreal height = qMin(_boundingRect.adjusted(0,0,-1,-1).height(), _boundingRect.adjusted(0,0,-1,-1).width());
        QRectF square(0, 0, height, height);
        square.moveCenter(_boundingRect.center());
        // HACK circle draw is 1 pixel larger than needed
        item = _scene->addEllipse(square, QPen(color), QBrush(Qt::NoBrush));
    }
        break;
    case 3:
    {
        const qreal height = qMin(_boundingRect.adjusted(0,0,-1,-1).height(), _boundingRect.adjusted(0,0,-1,-1).width());
        QRectF square(0, 0, height, height);
        square.moveCenter(_boundingRect.center());
        // HACK square draw is 1 pixel larger than needed
        item = _scene->addRect(square, QPen(color), QBrush(Qt::NoBrush));
    }
        break;
    }

    const unsigned int style = _generatorStyle->currentItem()->real();
    switch(style)
    {
    case 0:
        item->setTransformOriginPoint(_boundingRect.center());
        break;
    case 1:
    {
        item->setTransformOriginPoint(_boundingRect.center());
        item->setPos(qrandPointF()-_boundingRect.center());
    }
        break;
    case 2:
    {
        item->setTransformOriginPoint(qrandPointF());
    }
        break;
    case 3:
    {
        const QPointF randPoint = qrandPointF();
        item->setPos(randPoint-_boundingRect.center());
        item->setTransformOriginPoint(randPoint);
    }
        break;
    case 4:
    {
        item->setPos(qrandPointF()-_boundingRect.center());
        item->setTransformOriginPoint(qrandPointF());
    }
        break;
    }

    const unsigned int duration = _beatDuration->loopSizeInPpqn();
    MinoAnimatedItem maItem (uppqn, duration, item);
    _itemGroup.addToGroup(item);
    _animatedItems.append(maItem);
}

void MinaExpandingObjects::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    processNotesEvents(uppqn);
    processItemCreation(uppqn);

    _ecrScale.setEasingCurve(_generatorCurve->easingCurveType());

    if (_enabled && _beatFactor->isBeat(gppqn))
    {
        createItem(uppqn, _color->color());
    }
    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        const qreal progress = item.progressForUppqn(uppqn);
        if (progress >= 1.0)
        {
            _scene->removeItem(item._graphicsItem);
            _animatedItems.removeAt(i);
            delete item._graphicsItem;
        }
        else
        {
            _animatedItems.at(i)._graphicsItem->setScale(_ecrScale.valueForProgress(progress));
        }
    }

    if(!_enabled && !_animatedItems.count())
    {
        MinoAnimation::setAlive(false);
        _alive = false;
    }
}

void MinaExpandingObjects::_startNote(const uint uppqn, const quint8 note, const quint8 value)
{
    (void)value;
    createItem(uppqn, noteToColor(note, _color->color().hueF(), _color->color().saturationF()));
}
