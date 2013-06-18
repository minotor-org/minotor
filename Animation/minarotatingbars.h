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

#ifndef MINAROTATINGBARS_H
#define MINAROTATINGBARS_H

#include "minoanimation.h"

#include "minopropertyreal.h"
#include "minopropertyeasingcurve.h"
#include "easingcurvedreal.h"

class MinaRotatingBars : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRotatingBars(QObject *object);
    ~MinaRotatingBars();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Rotating bars", "Beat-sync rotating bars", QPixmap(":/images/rotatingbars.png"), MinaRotatingBars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    void setWidth(const qreal width) { _width->setValue(width); }
    void setLength(const qreal length) { _length->setValue(length); }
    void setEasingCurveType(const QEasingCurve::Type &type) { _generatorCurve->setEasingCurveType(type); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    QGraphicsRectItem *_items[4];
    MinoPropertyEasingCurve *_generatorCurve;
    MinoPropertyReal *_width;
    MinoPropertyReal *_length;
    int _maxPixels;
    QRectF _drawingZone;
    EasingCurvedReal _ecrAngle;

};

#endif // MINAROTATINGBARS_H
