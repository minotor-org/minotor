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

#include "minacurve.h"

#include <QDebug>
#include <QGraphicsBlurEffect>

MinaCurve::MinaCurve(QObject *object) :
    MinoAnimation(object)
{
    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    _generatorAccel = new MinoPropertyEasingCurve(this);
    _generatorAccel->setObjectName("acceleration");
    _generatorAccel->setLabel("Accel.");
    // Please note that curve have not been tested with all parameters:
    // Theses following lines are here to show how to add curve type to selector
    _generatorAccel->addEasingCurveType(QEasingCurve::Linear);
    _generatorAccel->addEasingCurveType(QEasingCurve::OutInBack);
    _generatorAccel->addEasingCurveType(QEasingCurve::InOutBounce);
    _generatorAccel->addEasingCurveType(QEasingCurve::InOutQuart);
    _generatorAccel->setEasingCurveType(QEasingCurve::Linear);

    QColor color;
    color.setHsvF(0.4, 1.0, 1.0);
    for (int i=0;i<_boundingRect.width();i++)
    {
        _items.append(_scene->addLine(i,_boundingRect.height(),i,5,QPen(color)));
        _itemGroup.addToGroup(_items[i]);
    }

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(1.1);
    _itemGroup.setGraphicsEffect(blur);
    _itemGroup.setVisible(false);
}

MinaCurve::~MinaCurve()
{
    //delete _itemGroup;
}

void MinaCurve::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    QColor color = _color->color();

    //Drawing curve
    QEasingCurve ecDraw(_generatorCurve->easingCurveType());

    //Animation curve
    QEasingCurve ecAnimation(_generatorAccel->easingCurveType());

    for (int i=0;i<_boundingRect.width();i++)
    {
        qreal pos = (qreal)i/(qreal)_boundingRect.width();
        qreal curvepos;
        pos += ecAnimation.valueForProgress(_beatFactor->progressForGppqn(gppqn));
        if(pos>1.0) pos -= 1.0;
        if(pos<=0.5)
        {
            curvepos = pos*2.0;
        }
        else
        {
            curvepos = (0.5-(pos-0.5))*2.0;
        }
        qreal curvefactor = ecDraw.valueForProgress(curvepos);
        _items[i]->setLine(i,_boundingRect.height()/2,i,(_boundingRect.height()*curvefactor));
        _items[i]->setPen(QPen(color));
    }
}

