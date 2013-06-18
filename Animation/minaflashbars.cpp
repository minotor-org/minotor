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

#include "minaflashbars.h"

#include <QDebug>

MinaFlashBars::MinaFlashBars(QObject *object) :
    MinoAnimation(object)
{
    _ecrAlpha.setStartValue(1.0);
    _ecrAlpha.setEndValue(0.0);
    _ecrAlpha.setEasingCurve(QEasingCurve::OutBounce);

    _width = new MinoPropertyReal(this);
    _width->setObjectName("width");
    _width->setLabel("Width");
    _width->setValue(0.2);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    int posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();
    _oldPosY = posY;
    _itemGroup.addToGroup(_scene->addRect(0, posY, _boundingRect.width(), _width->value()*_boundingRect.height(), QPen(Qt::NoPen),QBrush(_color->color())));
}

void MinaFlashBars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    QColor color = _color->color();

    _ecrAlpha.setEasingCurve(_generatorCurve->easingCurveType());

    color.setAlphaF(_ecrAlpha.valueForProgress(_beatFactor->progressForGppqn(gppqn)));

    QColor transparency;
    transparency.setAlpha(0);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
        static_cast<QGraphicsRectItem*>(item)->setBrush(color);
    }

    if (_beatFactor->isBeat(gppqn))
    {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }
        int posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();

        //TODO : find a better way to avoid identical consecutive position
        if (_width->value()*_boundingRect.height()<_boundingRect.height()-1)
        {
            while (posY == _oldPosY)
            {
                posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();
            }
        }
        _oldPosY = posY;
        _itemGroup.addToGroup(_scene->addRect(0, posY, _boundingRect.width(), _width->value()*(_boundingRect.height()-1.0)+1.0, QPen(Qt::NoPen),QBrush(color)));
    }
}
