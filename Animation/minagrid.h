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

#ifndef MINAGRID_H
#define MINAGRID_H

#include "minoinstrumentedanimation.h"

#include "minopropertyreal.h"
#include "minoitemizedproperty.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaGrid : public MinoInstrumentedAnimation
{
    Q_OBJECT
public:
    explicit MinaGrid(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Flashing grid", "Beat-synced flashing grid", QPixmap(":/images/grid.png"), MinaGrid::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    void setWidth(qreal density) { _width->setValue(density); }
    QGraphicsItem *graphicItem() { return &_itemGroup; }

signals:
    
public slots:

private:
    MinoPropertyReal *_width;
    MinoPropertyBeat *_beatDuration;
    MinoItemizedProperty *_itemsX;
    MinoItemizedProperty *_itemsY;
    MinoPropertyEasingCurve *_generatorCurve;
    EasingCurvedReal _ecrAlpha;

    int _oldPosY;

    // Graphics
    QGraphicsItemGroup _itemGroup;

    // Items
    MinoAnimatedItems _animatedItems;

    void createItem(const uint uppqn, const QColor &color, const unsigned int &pos);

    enum ItemData { Color };

    void _startNote(const uint uppqn, const quint8 note, const quint8 value);
    void _createItem(const uint uppqn);

    unsigned int noteToPos(int note);

    unsigned int _lastRand;
};

#endif // MINAFLASHBARS_H
