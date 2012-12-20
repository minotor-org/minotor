#include "minoanimationexpandingobjects.h"

MinoAnimationExpandingObjects::MinoAnimationExpandingObjects(Minotor *parent) :
    MinoAnimation(parent)
{
    _animatedProperty.setStartValue(QVariant(3.0));
    _animatedProperty.setEndValue(QVariant(0.1));

    //_animation.setEasingCurve(QEasingCurve::InBounce);

    // TODO Remove hardcoded values!
    _itemGroup.addToGroup(parent->scene()->addRect(QRectF(90, 50, 60, 60), QColor::fromRgbF(0.5, 0, 1, 1)));
    _itemGroup.addToGroup(parent->scene()->addRect(QRectF(100, 60, 40, 40), QColor::fromRgbF(0, 0.5, 1, 1)));
    // TODO Remove hardcoded values!
    _itemGroup.setTransformOriginPoint(120,80);

}

void MinoAnimationExpandingObjects::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    _itemGroup.setScale(_animatedProperty.currentValue().toReal());

    //foreach(QGraphicsItem* item, _mainItemGroup.childItems ())
    //{
    //item->rotate((360/12)/4);
    //}
}
