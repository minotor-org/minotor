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

#ifndef MINAFALLINGOBJECTS_H
#define MINAFALLINGOBJECTS_H

#include "minoanimation.h"

#include "easingcurvedreal.h"

#include "minopropertyreal.h"
#include "minopropertyeasingcurve.h"
#include "minoitemizedproperty.h"

class MinaFallingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaFallingObjects(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Falling objects", "Beat-sync moving lines", QPixmap(":/images/falling.png"), MinaFallingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

    bool isAlive() const { return _alive; }
    
signals:
    
public slots:

protected:
    MinoPropertyBeat *_beatDuration;
    MinoItemizedProperty *_generatorDirection;
    MinoPropertyReal *_generatorLength;
    MinoPropertyReal *_generatorDensity;
    MinoPropertyEasingCurve *_generatorCurve;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
    EasingCurvedReal _ecrPosition;
    bool _alive;

    void setAlive(const bool on);

    enum ItemData { Direction };
};
#endif // MINAFALLINGOBJECTS_H
