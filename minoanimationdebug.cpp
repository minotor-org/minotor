#include "minoanimationdebug.h"
#include <QColor>
#include <QDebug>

MinoAnimationDebug::MinoAnimationDebug(MinoChannel *channel) :
    MinoAnimation(QString("Debug"), channel)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _r.setObjectName("Red");
    _r.setValue(1.0);
    _g.setObjectName("Green");
    _b.setObjectName("Blue");
    _properties.append(&_r);
    _properties.append(&_g);
    _properties.append(&_b);
}

void MinoAnimationDebug::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color(Qt::blue);
    color.setRed(_r.value()*255);
    color.setGreen(_g.value()*255);
    color.setBlue(_b.value()*255);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
        delete item;
    }
    _itemGroup.addToGroup(_scene->addRect(_boundingRect, QPen(color),QBrush(color) ));
    _itemGroup.setOpacity(_beatAnimatedProperty.currentValue().toFloat());
}
