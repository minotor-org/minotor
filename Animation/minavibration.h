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

#ifndef MINAVIBRATION_H
#define MINAVIBRATION_H

#include "minoanimation.h"

#include "minopropertyreal.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaVibration : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaVibration(QObject *object);

    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Vibration", "Beat-synced vibration", QPixmap(":/images/vibration.png"), MinaVibration::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }
    
    void setSegments(qreal value) { _segments->setValue(value); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:

public slots:

private:
    QGraphicsItemGroup _itemGroup;
    EasingCurvedReal _ecrBarLenght;
    MinoPropertyEasingCurve *_generatorCurve;

protected:
    MinoPropertyReal *_segments;

};


#endif // MINAVIBRATION_H
