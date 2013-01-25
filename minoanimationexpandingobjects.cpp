#include "minoanimationexpandingobjects.h"

#include <QDebug>

MinoAnimationExpandingObjects::MinoAnimationExpandingObjects(QGraphicsScene *scene, QObject *parent) :
    MinoAnimation(QString("Expanding objects"), scene, parent)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.2));
    _beatAnimatedProperty.setEndValue(QVariant(8.0));

    _color.setObjectName("Color");
    _properties.append(&_color);
    _itemGroup.setTransformOriginPoint(_scene->sceneRect().width()/2,_scene->sceneRect().height()/2);
}

void MinoAnimationExpandingObjects::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    computeAnimaBeatProperty(gppqn);

    if (ppqn%24==0)
    {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }
        int x = _scene->sceneRect().width()/2;
        int y = _scene->sceneRect().height()/2;
        _itemGroup.addToGroup(_scene->addEllipse(x-(y),y-(y),_scene->sceneRect().height(),_scene->sceneRect().height(),QPen(Qt::green),QBrush(Qt::NoBrush)));
    }

    _itemGroup.setScale(_beatAnimatedProperty.currentValue().toReal());
}
