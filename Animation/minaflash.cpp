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

#include "minaflash.h"

#include <QColor>
#include <QDebug>

MinaFlash::MinaFlash(QObject *object):
    MinoAnimation(object),
    _rectItem(NULL)
{
    _ecrOpacity.setEasingCurve(QEasingCurve::OutBounce);
    _ecrOpacity.setStartValue(1.0);
    _ecrOpacity.setEndValue(0.0);
    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    QColor color;
    color.setHsvF(0.4, 1.0, 1.0);
    _rectItem = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _rectItem->setVisible(false);
}

MinaFlash::~MinaFlash()
{
    //delete _rectItem;
}

void MinaFlash::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    QColor color = _color->color();

    _ecrOpacity.setEasingCurve(_generatorCurve->easingCurveType());

    _rectItem->setBrush(QBrush(color));
    _rectItem->setOpacity(_ecrOpacity.valueForProgress(_beatFactor->progressForGppqn(gppqn)));
}
