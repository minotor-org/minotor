#include "minoanimationwaveform.h"
#include <QDebug>

MinoAnimationWaveform::MinoAnimationWaveform(QGraphicsScene *scene, QObject *parent) :
    MinoAnimation(QString("Waveform"), scene, parent)
{
    _beatAnimatedProperty.setStartValue(QVariant(_scene->sceneRect().height()/2));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _beatFactor.setObjectName("Beat factor");
    _properties.append(&_beatFactor);
    _beatFactor.setValue(0.4); // BeatFactor 1

    _color.setObjectName("Color");
    _properties.append(&_color);

}

void MinoAnimationWaveform::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    computeAnimaBeatProperty(gppqn);

    QColor color(Qt::green);
    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    for (int i=0;i<_scene->sceneRect().width();i++)
    {
        int currentAnimatedPropertyValue = 1+(qrand()%_beatAnimatedProperty.currentValue().toInt());
        _itemGroup.addToGroup(_scene->addLine(i,((_scene->sceneRect().height()/2)-currentAnimatedPropertyValue),i,((_scene->sceneRect().height()/2)+currentAnimatedPropertyValue),QPen(color)));
    }

}
