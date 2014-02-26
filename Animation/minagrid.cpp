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

#include "minagrid.h"

#include <QDebug>

MinaGrid::MinaGrid(QObject *object) :
    MinoInstrumentedAnimation(object)
{
    _ecrAlpha.setStartValue(1.0);
    _ecrAlpha.setEndValue(0.0);
    _ecrAlpha.setEasingCurve(QEasingCurve::OutBounce);

    _width = new MinoPropertyReal(this);
    _width->setObjectName("width");
    _width->setLabel("Width");
    _width->setValue(0.2);

    _itemsX = new MinoItemizedProperty(this);
    _itemsX->setObjectName("itemsx");
    _itemsX->setLabel("Items X");
    _itemsX->addItem("2", 2);
    _itemsX->addItem("4", 4);
    _itemsX->addItem("8", 8);
    _itemsX->addItem("12", 12);
    _itemsX->addItem("16", 16);
    _itemsX->addItem("20", 20);
    _itemsX->addItem("24", 24);
    _itemsX->addItem("28", 28);
    _itemsX->addItem("32", 32);
    _itemsX->setCurrentItemFromString("8");

    _itemsY = new MinoItemizedProperty(this);
    _itemsY->setObjectName("itemsy");
    _itemsY->setLabel("Items Y");
    _itemsY->addItem("2", 2);
    _itemsY->addItem("4", 4);
    _itemsY->addItem("8", 8);
    _itemsY->addItem("12", 12);
    _itemsY->addItem("16", 16);
    _itemsY->addItem("20", 20);
    _itemsY->addItem("24", 24);
    _itemsY->addItem("28", 28);
    _itemsY->addItem("32", 32);
    _itemsY->setCurrentItemFromString("2");

    _beatDuration = new MinoPropertyBeat(this, true);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    _lastRand = 0;
}

void MinaGrid::_createItem(const uint uppqn)
{
    int rand = qrandF() * _itemsX->currentItem()->real() * _itemsY->currentItem()->real();
    while (_lastRand == rand)
    {
        rand = qrandF() * _itemsX->currentItem()->real() * _itemsY->currentItem()->real();
    }
    _lastRand = rand;
    createItem(uppqn, _color->color(), rand);
}

void MinaGrid::createItem(const uint uppqn, const QColor& color, const unsigned int& pos)
{
    qreal itemsX = _itemsX->currentItem()->real();
    qreal itemsY = _itemsY->currentItem()->real();
    unsigned int x = pos%(int)itemsX;
    unsigned int y = pos/itemsX;
    const unsigned int duration = _beatDuration->loopSizeInPpqn();
    QGraphicsItem *item = NULL;
    qreal itemWidth =(qreal)_boundingRect.width()/itemsX;
    qreal itemHeight = (qreal)_boundingRect.height()/itemsY;
    item = _scene->addRect(x*itemWidth, y*itemHeight, itemWidth, itemHeight, QPen(Qt::NoPen),QBrush(QColor(127,127,0)));
    item->setData(MinaGrid::Color, color);
    MinoAnimatedItem maItem (uppqn, duration, item);
    _itemGroup.addToGroup(item);
    _animatedItems.append(maItem);
}

void MinaGrid::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    processNotesEvents(uppqn);
    processItemCreation(uppqn);

    if (_enabled && _beatFactor->isBeat(gppqn))
    {
        _createItem(uppqn);
    }

    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        if (item.isCompleted(uppqn))
        {
            delete item._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal progress = item.progressForUppqn(uppqn);
            QColor color = item.graphicsItem()->data(MinaGrid::Color).value<QColor>();
            _ecrAlpha.setEasingCurve(_generatorCurve->easingCurveType());
            color.setAlphaF(_ecrAlpha.valueForProgress(progress));

            static_cast<QGraphicsRectItem*>(item.graphicsItem())->setBrush(color);

        }
    }

    if(!_enabled && !_animatedItems.count())
    {
        MinoAnimation::setAlive(false);
        _alive = false;
    }
}

void MinaGrid::_startNote(const uint uppqn, const quint8 note, const quint8 value)
{
    (void)value;
    createItem(uppqn, noteToColor(note,_color->color()), noteToPos(note));
}

unsigned int MinaGrid::noteToPos(int note)
{
    qreal itemsX = _itemsX->currentItem()->real();
    qreal itemsY = _itemsY->currentItem()->real();
    qreal totalItems = itemsX * itemsY;

    unsigned int subnote=0;

    if(totalItems>=48)
    {
        subnote = note % 48;
    }
    else if (totalItems>=36)
    {
        subnote = note % 36;
    }
    else if (totalItems>=24)
    {
        subnote = note % 24;
    }
    else
    {
        subnote = note % 12;
    }
    return subnote;

}
