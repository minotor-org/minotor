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
    MinoInstrumentedAnimation(object)
{
    _ecrAlpha.setStartValue(1.0);
    _ecrAlpha.setEndValue(0.0);
    _ecrAlpha.setEasingCurve(QEasingCurve::OutBounce);

    _width = new MinoPropertyReal(this);
    _width->setObjectName("width");
    _width->setLabel("Width");
    _width->setValue(0.2);

    _beatDuration = new MinoPropertyBeat(this, true);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaFlashBars::_createItem(const uint uppqn)
{
    createItem(uppqn, _color->color());
}

void MinaFlashBars::createItem(const uint uppqn, const QColor& color)
{
    QGraphicsItem *item = NULL;
    const unsigned int duration = _beatDuration->loopSizeInPpqn();
    int posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height();
    //while (_oldPosY == posY = qMin(qrandF(),(1.0-(_width->value())))*_boundingRect.height())

    item = _scene->addRect(0, posY, _boundingRect.width(), _width->value()*_boundingRect.height(), QPen(Qt::NoPen),QBrush(color));
    item->setData(MinaFlashBars::Color, color);
    MinoAnimatedItem maItem (uppqn, duration, item);
    _itemGroup.addToGroup(item);
    _animatedItems.append(maItem);
}

void MinaFlashBars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    processNotesEvents(uppqn);
    processItemCreation(uppqn);

    if (_enabled && _beatFactor->isBeat(gppqn))
    {
        createItem(uppqn, _color->color());
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
            QColor color = item.graphicsItem()->data(MinaFlashBars::Color).value<QColor>();
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

void MinaFlashBars::_startNote(const uint uppqn, const quint8 note, const quint8 value)
{
    (void)value;
    createItem(uppqn, noteToColor(note));
}
