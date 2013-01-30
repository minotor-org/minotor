#include "minaexpandingobjects.h"

#include <QDebug>

MinaExpandingObjects::MinaExpandingObjects(Minotor *minotor):
    MinoAnimation(minotor)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.01));

    _color.setObjectName("Color");
    _properties.append(&_color);
    _itemGroup.setTransformOriginPoint(_boundingRect.center());
}

void MinaExpandingObjects::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    if (ppqn==0)
    {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }
        // Be sure scale factor is 1.0 when we draw new items
        _itemGroup.setScale(1.0);
        _itemGroup.addToGroup(_scene->addEllipse(_boundingRect, QPen(Qt::green), QBrush(Qt::NoBrush)));
    }
    _itemGroup.setScale(_beatAnimatedProperty.currentValue().toReal());
    qDebug() << "scale" << _itemGroup.scale();
}
