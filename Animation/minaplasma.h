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

#ifndef MINAPLASMA_H
#define MINAPLASMA_H

#include "minoanimation.h"

#include <QGraphicsRectItem>

#include "minopropertyeasingcurve.h"
#include "easingcurvedreal.h"
#include "minoitemizedproperty.h"

class MinaPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaPlasma(QObject *object);
    ~MinaPlasma();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Plasma", "Strange thing ;-)", QPixmap(":/images/plasma.png"), MinaPlasma::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    QGraphicsRectItem *_rectBackground;
    QGraphicsRectItem *_rectTopLeft;
    QGraphicsRectItem *_rectTopRight;
    QGraphicsRectItem *_rectBottomLeft;
    QGraphicsRectItem *_rectBottomRight;
    MinoPropertyEasingCurve *_generatorCurve;
    MinoItemizedProperty *_generatorSteps;
    EasingCurvedReal _ecrPosition;
};

#endif // MINAPLASMA_H
