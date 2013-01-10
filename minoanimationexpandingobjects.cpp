#include "minoanimationexpandingobjects.h"

MinoAnimationExpandingObjects::MinoAnimationExpandingObjects(QGraphicsScene *scene, QObject *parent) :
    MinoAnimation(QString("Expanding objects"), scene, parent)
{
    _animatedProperty.setStartValue(QVariant(3.0));
    _animatedProperty.setEndValue(QVariant(0.1));

    //_animation.setEasingCurve(QEasingCurve::InBounce);
    // TODO Remove hardcoded values!
    _itemGroup.addToGroup(scene->addRect(QRectF(90, 50, 60, 60), QColor::fromRgbF(1.0, 1.0, 0.0, 1.0)));
    _itemGroup.addToGroup(scene->addRect(QRectF(100, 60, 40, 40), QColor::fromRgbF(0.5, 1.0, 0.0, 1.0)));
    // TODO Remove hardcoded values!
    _itemGroup.setTransformOriginPoint(120,80);

    _rotation.setObjectName("Rotation");
    _properties.append(&_rotation);
}

void MinoAnimationExpandingObjects::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    _itemGroup.setScale(_animatedProperty.currentValue().toReal());

    _itemGroup.setRotation(_rotation.value()*360);
/*
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
      item->rotate(*((360/12)/4));
    }
    */
}
