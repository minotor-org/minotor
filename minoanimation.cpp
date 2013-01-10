#include "minoanimation.h"

MinoAnimation::MinoAnimation(QString name, QGraphicsScene *scene, QObject *parent) :
    QObject(parent),
    _scene(scene),
    _name(name)
{
}

void MinoAnimation::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    _itemGroup.setScale(_animatedProperty.currentValue().toReal());
}
