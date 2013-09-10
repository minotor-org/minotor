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

#include "minabarsfromsides.h"
#include <QColor>
#include <QDebug>

MinaBarsFromSides::MinaBarsFromSides(QObject *object) :
    MinoAnimation(object)
{
    _ecr.setStartValue(1.0);
    _ecr.setEndValue(0.0);
    _ecr.setEasingCurve(QEasingCurve::OutBounce);

    _colorType = new MinoItemizedProperty(this);
    _colorType->setObjectName("color-type");
    _colorType->setLabel("Color type");
    _colorType->addItem("plain", 0);
    _colorType->addItem("grad.", 1);
    _colorType->setCurrentItemFromString("plain");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaBarsFromSides::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    // Colors
    QColor color, colorMin, colorMax;
    color = _color->color();

    _ecr.setEasingCurve(_generatorCurve->easingCurveType());

    QLinearGradient gradH(0.0, 0.0, 0.0, (qreal)_boundingRect.height()) ;
    QLinearGradient gradV(0.0, 0.0, (qreal)_boundingRect.width(), 0.0) ;

    if (_colorType->currentItem()->real() == 0.0)
    {
        gradH.setColorAt(0.0, color) ;
        gradH.setColorAt(0.5, Qt::transparent) ;
        gradH.setColorAt(1.0, color) ;

        gradV.setColorAt(0.0, color) ;
        gradV.setColorAt(0.5, Qt::transparent) ;
        gradV.setColorAt(1.0, color) ;
    }
    else
    {
        qreal minValue = color.hueF()-0.15; if(minValue<0.0) minValue += 1.0;
        colorMin.setHsvF(minValue, color.saturationF(), color.valueF());

        qreal maxValue = color.hueF()+0.15; if(maxValue>1.0) maxValue -= 1.0;
        colorMax.setHsvF(maxValue, color.saturationF(), color.valueF());

        gradH.setColorAt(0.0, colorMax) ;
        gradH.setColorAt(0.3, color) ;
        gradH.setColorAt(0.45, colorMin) ;
        gradH.setColorAt(0.55, colorMin) ;
        gradH.setColorAt(0.7, color) ;
        gradH.setColorAt(1, colorMax) ;

        gradV.setColorAt(0.0, colorMax) ;
        gradV.setColorAt(0.3, color) ;
        gradV.setColorAt(0.45, colorMin) ;
        gradV.setColorAt(0.55, colorMin) ;
        gradV.setColorAt(0.7, color) ;
        gradV.setColorAt(1, colorMax) ;
    }

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }

    const qreal position = _ecr.valueForProgress(_beatFactor->progressForGppqn(gppqn));
    if ((int)(gppqn/_beatFactor->loopSizeInPpqn())%2)
    {
        for (int i=0;i< _boundingRect.height();i++)
        {
            qreal lineLength = 0.5 + (qrandF() * position * _boundingRect.width() / 2.0);
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.left(), i, _boundingRect.left()+lineLength, i, QPen(QBrush(gradV),1)));
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.width()-lineLength, i, _boundingRect.width(), i, QPen(QBrush(gradV),1)));
        }
    }
    else
    {
        for (int i=0;i< _boundingRect.width();i++)
        {
            qreal lineLength = 0.5 + (qrandF() * position * _boundingRect.height() / 2.0);
            _itemGroup.addToGroup(_scene->addLine(i, _boundingRect.top(), i, _boundingRect.top()+lineLength, QPen(QBrush(gradH),1)));
            _itemGroup.addToGroup(_scene->addLine(i, (qreal)_boundingRect.height()-lineLength, i, _boundingRect.height(), QPen(QBrush(gradH),1)));
        }
    }
}
