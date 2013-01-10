#include "minoanimationdebug.h"
#include <QColor>
#include <QGraphicsScene>
#include <QDebug>

MinoAnimationDebug::MinoAnimationDebug(QGraphicsScene* scene, QObject *parent) :
    MinoAnimation(QString("Debug"), scene, parent)
{
    _animatedProperty.setStartValue(QVariant(255));
    _animatedProperty.setEndValue(QVariant(0));
    _animatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _r.setObjectName("Red");
    _g.setObjectName("Green");
    _b.setObjectName("Blue");
    _properties.append(&_r);
    _properties.append(&_g);
    _properties.append(&_b);
}

void MinoAnimationDebug::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    QColor color(Qt::blue);
    color.setRed(_r.value()*255);
    color.setGreen(_g.value()*255);
    color.setBlue(_b.value()*255);
    color.setAlpha(_animatedProperty.currentValue().toInt());

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
        delete item;
    }
    _itemGroup.addToGroup(_scene->addRect(QRectF(0, 0, 240, 160), QPen(color),QBrush(color) ));
}
