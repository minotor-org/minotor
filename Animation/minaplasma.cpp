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

#include "minaplasma.h"
#include "minoprogram.h"
#include <QDebug>

MinaPlasma::MinaPlasma(QObject *object):
    MinoAnimation(object)
{
    _ecrPosition.setStartValue(0.0);
    _ecrPosition.setEndValue(1.0);

    _generatorSteps = new MinoItemizedProperty(this);
    _generatorSteps->setObjectName("steps");
    _generatorSteps->setLabel("Steps");
    _generatorSteps->addItem("2", 2);
    _generatorSteps->addItem("4", 4);
    _generatorSteps->addItem("6", 6);
    _generatorSteps->addItem("8", 8);
    _generatorSteps->addItem("10", 10);
    _generatorSteps->addItem("12", 12);
    _generatorSteps->addItem("14", 14);
    _generatorSteps->setCurrentItem("4");
    _generatorSteps->setLinear();

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    QColor color = _color->color();

    //background
    _rectBackground = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    //gradient top left
    _rectTopLeft = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    //gradient top right
    _rectTopRight = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    //gradient bottom left
    _rectBottomLeft = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    //gradient bottom right
    _rectBottomRight = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));

    _itemGroup.addToGroup(_rectBackground);
    _itemGroup.addToGroup(_rectTopLeft);
    _itemGroup.addToGroup(_rectTopRight);
    _itemGroup.addToGroup(_rectBottomLeft);
    _itemGroup.addToGroup(_rectBottomRight);
    _itemGroup.setVisible(false);
}

MinaPlasma::~MinaPlasma()
{
    delete _rectBackground;
    delete _rectTopLeft;
    delete _rectTopRight;
    delete _rectBottomLeft;
    delete _rectBottomRight;
}

void MinaPlasma::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    QColor color1 = _color->color();

    QColor color2;
    qreal minValue1 = color1.hueF()-0.2; if(minValue1<0.0) minValue1 += 1.0;
    color2.setHsvF(minValue1, 1.0, 1.0);

    QColor color3;
    qreal maxValue1 = color1.hueF()+0.2; if(maxValue1>1.0) maxValue1 -= 1.0;
    color3.setHsvF(maxValue1, color1.saturationF(), 1.0);

    QColor color4;
    qreal minValue2 = color1.hueF()-0.4; if(minValue2<0.0) minValue2 += 1.0;
    color4.setHsvF(minValue2, color1.saturationF(), 0.96);

    QColor color5;
    qreal maxValue2 = color1.hueF()+0.4; if(maxValue2>1.0) maxValue2 -= 1.0;
    color5.setHsvF(maxValue2, color1.saturationF(), 0.9);

    QGradient grad1;
    QGradient grad2;
    QGradient grad3;
    QGradient grad4;
    QGradient grad5;

    //Linear gradient for background
    grad1 = QLinearGradient(QPointF(_boundingRect.topLeft().x()-_boundingRect.width(),_boundingRect.topLeft().y()),QPointF(_boundingRect.topRight().x()+_boundingRect.width(),_boundingRect.topLeft().y()));
    //Radial gradient from each corner (gradient is bigger than bounding rect to avoid color loop clipping)
    //Top left
    grad2 = QRadialGradient(QPointF(_boundingRect.topLeft().x()-_boundingRect.width(),_boundingRect.topLeft().y()-_boundingRect.height()),_boundingRect.width()*4);
    //Top right
    grad3 = QRadialGradient(QPointF(_boundingRect.topRight().x()+_boundingRect.width(),_boundingRect.topRight().y()-_boundingRect.height()),_boundingRect.width()*4);
    //Bottom left
    grad4 = QRadialGradient(QPointF(_boundingRect.topLeft().x()-_boundingRect.width(),_boundingRect.topLeft().y()+_boundingRect.height()),_boundingRect.width()*4);
    //Bottom right
    grad5 = QRadialGradient(QPointF(_boundingRect.topRight().x()+_boundingRect.width(),_boundingRect.topRight().y()+_boundingRect.height()),_boundingRect.width()*4);

    _ecrPosition.setEasingCurve(_generatorCurve->easingCurveType());

    const unsigned int waves = _generatorSteps->currentItem()->real();
    const qreal step = 1.0 / ((qreal) waves *2.0);

    const qreal anipos = _ecrPosition.valueForProgress(_beatFactor->progressForGppqn(gppqn));
    bool toogle = true;
    for (qreal pos = 0.0; pos <= 1.0; pos+=step)
    {
        qreal at = (anipos/waves) + pos;
        if(at > 1.0) at -= 1.0;

        if(toogle)
        {
            //Calculate an alternative color for background gradient
            QColor altColor;
            qreal maxValue = color1.hueF()+0.3;
            if(maxValue>1.0) maxValue -= 1.0;
            altColor.setHsvF(maxValue, 1.0, 1.0);
            grad1.setColorAt(at, altColor);

            QColor transparentColor;
            transparentColor.setHsv(1,1,1,0);
            grad2.setColorAt(at, transparentColor);
            grad3.setColorAt(at, transparentColor);
            grad4.setColorAt(at, transparentColor);
            grad5.setColorAt(at, transparentColor);
        }
        else
        {
            grad1.setColorAt(at, color1) ;
            grad2.setColorAt(at, color2) ;
            grad3.setColorAt(at, color3) ;
            grad4.setColorAt(at, color4) ;
            grad5.setColorAt(at, color5) ;
        }
        toogle = !toogle;
    }

    _rectBackground->setBrush(QBrush(grad1));
    _rectTopLeft->setBrush(QBrush(grad2));
    _rectTopRight->setBrush(QBrush(grad3));
    _rectBottomLeft->setBrush(QBrush(grad4));
    _rectBottomRight->setBrush(QBrush(grad5));

}
