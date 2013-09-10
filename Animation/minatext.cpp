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

#include "minatext.h"
#include <QLabel>

MinaText::MinaText(QObject *object) :
    MinoAnimation(object)
{
    _ecrScale.setStartValue(1.0);
    _ecrScale.setEndValue(0.01);

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
    _beatDuration->setCurrentItemFromString("1");
    _beatDuration->setLinear();

    _text = new MinoPropertyText(this);

    _generatorStyle = new MinoItemizedProperty(this);
    _generatorStyle->setObjectName("style");
    _generatorStyle->setLabel("Style");
    _generatorStyle->addItem("P:F T:F", 0);
    _generatorStyle->addItem("P:R T:F", 1);
    _generatorStyle->addItem("P:F T:R", 2);
    _generatorStyle->addItem("P:R=T:R", 3);
    _generatorStyle->addItem("P:R T:R", 4);
    _generatorStyle->setCurrentItemFromString("P:R T:F");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaText::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    QColor color = _color->color();

    _ecrScale.setEasingCurve(_generatorCurve->easingCurveType());

    if (_beatFactor->isBeat(gppqn))
    {
        QGraphicsTextItem* item = _scene->addText(_text->text(),QFont("Arial",12,QFont::Bold,false));
        QRectF tRect = item->boundingRect();
        tRect.adjust(0,0,-1,-1);
        tRect.moveCenter(_boundingRect.center());
        item->setPos(tRect.topLeft());
        item->setDefaultTextColor(color);

        const unsigned int style = _generatorStyle->currentItem()->real();
        switch(style)
        {
        case 0:
        {
            item->setTransformOriginPoint(_boundingRect.center());
        }
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
            item->setTransformOriginPoint(randPoint-_boundingRect.center());
        }
            break;
        case 4:
        {
            item->setPos(qrandPointF()-_boundingRect.center());
            item->setTransformOriginPoint(qrandPointF()-_boundingRect.center());
        }
            break;
        }

        const unsigned int duration = _beatDuration->currentItem()->real();
        MinoAnimatedItem maItem (uppqn, duration, item);
        _itemGroup.addToGroup(item);
        _animatedItems.append(maItem);

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
            _animatedItems.at(i)._graphicsItem->setScale(_ecrScale.valueForProgress(progress));
        }
    }
}

