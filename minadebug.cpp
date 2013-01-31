#include "minadebug.h"

#include <QColor>
#include <QDebug>

MinaDebug::MinaDebug(Minotor *minotor):
    MinoAnimation(minotor)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);


    _color.setObjectName("Color");
    _properties.append(&_color);
}

void MinaDebug::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
        delete item;
    }
    _itemGroup.addToGroup(_scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color) ));
    _itemGroup.setOpacity(_beatAnimatedProperty.currentValue().toFloat());
}
