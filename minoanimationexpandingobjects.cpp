#include "minoanimationexpandingobjects.h"

#include <QDebug>

MinoAnimationExpandingObjects::MinoAnimationExpandingObjects(MinoChannel *channel) :
    MinoAnimation(QString("Expanding objects"), channel)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.2));
    _beatAnimatedProperty.setEndValue(QVariant(8.0));

    _color.setObjectName("Color");
    _properties.append(&_color);
    _itemGroup.setTransformOriginPoint(_boundingRect.center());
}

void MinoAnimationExpandingObjects::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    if (ppqn%24==0)
    {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }
        int x = _boundingRect.width()/2;
        int y = _boundingRect.height()/2;
        _itemGroup.addToGroup(_scene->addEllipse(x-(y),y-(y),_boundingRect.height(),_boundingRect.height(),QPen(Qt::green),QBrush(Qt::NoBrush)));
    }

    _itemGroup.setScale(_beatAnimatedProperty.currentValue().toReal());
}
