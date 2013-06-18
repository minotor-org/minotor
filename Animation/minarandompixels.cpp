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

#include "minarandompixels.h"

#include <QDebug>

MinaRandomPixels::MinaRandomPixels(QObject *object) :
    MinoAnimation(object)
{
    _ecrAlpha.setStartValue(1.0);
    _ecrAlpha.setEndValue(0.0);
    _ecrAlpha.setEasingCurve(QEasingCurve::OutBounce);

    _density = new MinoPropertyReal(this);
    _density->setObjectName("density");
    _density->setLabel("Density");
    _density->setValue(0.1);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    _beatDuration = new MinoItemizedProperty(this);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");
    _beatDuration->addItem("1/4", 6);
    _beatDuration->addItem("1/2", 12);
    _beatDuration->addItem("1", 24);
    _beatDuration->addItem("2", 48);
    _beatDuration->addItem("4", 96);
    _beatDuration->addItem("8", 192);
    _beatDuration->addItem("16", 384);
    _beatDuration->setCurrentItem("1");
    _beatDuration->setLinear();
}

void MinaRandomPixels::createPixels(const unsigned int uppqn, const unsigned duration)
{
    const qreal pixelCount = _density->value()*(_boundingRect.width()*_boundingRect.height());
    for(int i=0; i<pixelCount; i++)
    {
        const QPointF rand = qrandPointF();
        const qreal h = 0.1;
        QGraphicsLineItem *gli = _scene->addLine(rand.x(), rand.y(), rand.x()+h, rand.y()+h, QPen(_color->color()));
        _itemGroup.addToGroup(gli);
        MinoAnimatedItem maItem (uppqn, duration, gli);
        _animatedItems.append(maItem);
    }
}

void MinaRandomPixels::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int, const unsigned int )
{
    _ecrAlpha.setEasingCurve(_generatorCurve->easingCurveType());

    if (_beatFactor->isBeat(gppqn))
    {
        createPixels(uppqn, (uint)_beatDuration->currentItem()->real());
    }

    // Animate pixels
    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        MinoAnimatedItem mai = _animatedItems.at(i);
        QGraphicsLineItem *gli = dynamic_cast<QGraphicsLineItem*>(mai.graphicsItem());
        Q_ASSERT(gli);
        if (mai.isCompleted(uppqn))
        {
            delete mai._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal progress = mai.progressForUppqn(uppqn);
            QColor color = gli->pen().color();
            color.setAlphaF(_ecrAlpha.valueForProgress(progress));
            gli->setPen(color);
        }
    }
}
