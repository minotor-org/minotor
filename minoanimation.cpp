#include "minoanimation.h"

MinoAnimation::MinoAnimation(Minotor *parent) :
    QObject(parent)
{
}

QGraphicsItemGroup* MinoAnimation::itemGroup()
{
    return &_itemGroup;
}

void MinoAnimation::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    _itemGroup.setScale(_animatedProperty.currentValue().toReal());

    //foreach(QGraphicsItem* item, _mainItemGroup.childItems ())
    //{
    //item->rotate((360/12)/4);
    //}
}
