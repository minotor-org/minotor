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

#include "minoinstrumentedanimation.h"

#include "easingcurvedreal.h"

#include "minopropertyreal.h"
#include "minopropertyeasingcurve.h"
#include "minoitemizedproperty.h"

class MinaFallingObjects : public MinoInstrumentedAnimation
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
    
signals:
    
protected:
    // ECR
    EasingCurvedReal _ecrPosition;

    // Properties
    MinoPropertyBeat *_beatDuration;
    MinoItemizedProperty *_generatorDirection;
    MinoPropertyReal *_generatorLength;
    MinoPropertyReal *_generatorWidth;
    MinoPropertyReal *_generatorDensity;
    MinoPropertyEasingCurve *_generatorCurve;

    // Graphics
    QGraphicsItemGroup _itemGroup;

    // Items
    MinoAnimatedItems _animatedItems;

    void createItem(const uint uppqn, const QColor &color, const unsigned int &pos, const unsigned int &direction);

    enum ItemData { Direction };

    void _startNote(const uint uppqn, const quint8 note, const quint8 value);
    void _createItem(const uint uppqn);
};
#endif // MINAFALLINGOBJECTS_H
