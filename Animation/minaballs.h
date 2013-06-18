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

#ifndef MINABALLS_H
#define MINABALLS_H

#include "minoanimation.h"
#include "minopropertyreal.h"

#include <QGraphicsRectItem>
#include <QLineF>
#include <QList>

class MinoAnimatedBall : public MinoAnimatedItem
{
public:
    explicit MinoAnimatedBall(QLineF path, QLineF destination, const unsigned int startUppqn,const unsigned int duration,QGraphicsItem *graphicsItem):
        MinoAnimatedItem(startUppqn, duration, graphicsItem),
        _path(path),
        _destination(destination)
    {
    }

    QLineF _path;
    QLineF _destination;
} ;

class MinaBalls : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaBalls(QObject *object);

    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Balls", "Balls", QPixmap(":/images/balls.png"), MinaBalls::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItemGroup* graphicItem() { return &_itemGroup; }

    //void setDuration(const unsigned int duration) { _beatDuration.setCurrentItem(QString::number(duration)); }
private:
    QGraphicsItemGroup _itemGroup;
    QList<MinoAnimatedBall> _animatedItems;
    QList<QLineF> _borders;

    MinoPropertyReal *_ballSize;

};

#endif // MINABALLS_H
