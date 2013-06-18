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

#include "minavibration.h"
#include <QDebug>
#include <QGraphicsBlurEffect>

MinaVibration::MinaVibration(QObject *object) :
    MinoAnimation(object)
{
    _ecrBarLenght.setStartValue((qreal)_boundingRect.height()/2.0);
    _ecrBarLenght.setEndValue(1.0);
    _ecrBarLenght.setEasingCurve(QEasingCurve::OutBounce);

    _segments = new MinoPropertyReal(this);
    _segments->setObjectName("segment");
    _segments->setLabel("Segment");
    _segments->setValue(0.3);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaVibration::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    // Colors
    QColor color = _color->color();

    _ecrBarLenght.setEasingCurve(_generatorCurve->easingCurveType());
    QEasingCurve ec(_generatorCurve->easingCurveType());
    color.setAlphaF(1.0-ec.valueForProgress(_beatFactor->progressForGppqn(gppqn)));


    const qreal middle = (qreal)_boundingRect.height()/2;
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    qreal currentX = -1;
    qreal currentY = middle;
    const int maxSegments = _boundingRect.width()/2;
    qreal barLenghtFactor = _ecrBarLenght.valueForProgress(_beatFactor->progressForGppqn(gppqn));

    while (currentX < _boundingRect.width())
    {
        //line up
        qreal randY = middle + (qrandF() * barLenghtFactor * 0.9);
        qreal randX = currentX + qrandF()* qreal(_boundingRect.width()/(1+(_segments->value()*maxSegments)));

        _itemGroup.addToGroup(_scene->addLine(currentX, currentY, randX, randY, QPen(color)));

        currentX = randX;
        currentY = randY;

        //line down
        randY = middle - (qrandF() * barLenghtFactor * 0.9);
        randX = currentX + (int)(qrandF()* qreal(_boundingRect.width()/(1+(_segments->value()*maxSegments))));

        _itemGroup.addToGroup(_scene->addLine(currentX, currentY, randX, randY, QPen(color)));

        currentX = randX;
        currentY = randY;
    }
}
