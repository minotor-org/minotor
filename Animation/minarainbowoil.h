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

#ifndef MINARAINBOWOIL_H
#define MINARAINBOWOIL_H

#include "minoanimation.h"

#include <QWidget>
#include <QPainter>
#include <QImage>

#include "minopropertyreal.h"
#include "minopropertyeasingcurve.h"

class OilImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OilImageWidget(QWidget *parent = 0);

    void setImage(QImage *image);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    QImage *_image;
};


class MinaRainbowOil : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRainbowOil(QObject *parent);
    ~MinaRainbowOil();

    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Rainbow Oil", "Beat-sync moving colored oil", QPixmap(":/images/rainbowoil.png"), MinaRainbowOil::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:

protected:
    void renderImage(const qreal pos, const qreal hue, const qreal light, QImage *image);
    
private:
    QGraphicsItemGroup _itemGroup;
    OilImageWidget *_oilImageWidget;
    MinoPropertyEasingCurve *_generatorCurve;
    MinoItemizedProperty *_style;
    MinoPropertyReal *_mprSpeed;
    MinoPropertyReal *_mprBoost;
    MinoPropertyReal *_mprStep;

    QImage *_image;
    qreal _pos;

};

#endif // MINARAINBOWOIL_H
