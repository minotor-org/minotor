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

#include "minastars.h"

MinaStars::MinaStars(QObject *object) :
    MinoInstrumentedAnimation(object)
{
    _ecrPosition.setStartValue(0.0);
    _ecrPosition.setEndValue(2.0);
    _ecrPosition.setEasingCurve(QEasingCurve::InQuart);

    _beatDuration = new MinoPropertyBeat(this, true);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    _generatorDensity = new MinoPropertyReal(this);
    _generatorDensity->setObjectName("density");
    _generatorDensity->setLabel("Density");
    _generatorDensity->setValue(0.4);

}

void MinaStars::createItem(const unsigned int uppqn, const QColor& color)
{
    const QPointF offset(0.1, 0.1);
    QGraphicsItemGroup *group = new QGraphicsItemGroup(&_itemGroup, _scene);
    QGraphicsItem *item = NULL;
    const unsigned int density = qMax(1.0,(_generatorDensity->value()*qMax(_boundingRect.width(),_boundingRect.height())));
    const unsigned int duration = _beatDuration->loopSizeInPpqn();

    for (unsigned int i=0;i<density;i++)
    {
        QPointF randPoint = qrandPointF();

        const QLineF line1(randPoint-offset, randPoint+offset);
        item = _scene->addLine(line1, QPen(color));
        group->addToGroup(item);
        // Mirror on X
        randPoint.setX(_boundingRect.width()-randPoint.x());
        const QLineF line2(randPoint-offset, randPoint+offset);
        item = _scene->addLine(line2, QPen(color));
        group->addToGroup(item);
        // Mirror on Y
        randPoint.setY(_boundingRect.height()-randPoint.y());
        const QLineF line3(randPoint-offset, randPoint+offset);
        item = _scene->addLine(line3, QPen(color));
        group->addToGroup(item);
        // Mirror on X
        randPoint.setX(_boundingRect.width()-randPoint.x());
        const QLineF line4(randPoint-offset, randPoint+offset);
        item = _scene->addLine(line4, QPen(color));
        group->addToGroup(item);
    }
    group->setTransformOriginPoint(_boundingRect.center());
    MinoAnimatedItem maItem (uppqn, duration, group);
    _animatedItems.append(maItem);
}

void MinaStars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    processNotesEvents(uppqn);
    processItemCreation(uppqn);

    _ecrPosition.setEasingCurve(_generatorCurve->easingCurveType());

    if (_enabled && _beatFactor->isBeat(gppqn))
    {
        createItem(uppqn, _color->color());
    }

    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        if (uppqn > (item._startUppqn+item._duration))
        {
            delete item._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal progress = item.progressForUppqn(uppqn);
            item._graphicsItem->setScale(_ecrPosition.valueForProgress(progress));
            item._graphicsItem->setOpacity((_ecrPosition.valueForProgress(progress)/2));
        }
    }

    if(!_enabled && !_animatedItems.count())
    {
        MinoAnimation::setAlive(false);
        _alive = false;
    }
}

void MinaStars::_createItem(const uint uppqn)
{
    createItem(uppqn, _color->color());
}

void MinaStars::_startNote(const uint uppqn, const quint8 note, const quint8 value)
{
    (void)value;
    createItem(uppqn, noteToColor(note, _color->color()));
}
