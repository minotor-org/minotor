#include "minoanimationwaveform.h"
#include <QDebug>

MinoAnimationWaveform::MinoAnimationWaveform(MinoChannel *channel) :
    MinoAnimation(QString("Waveform"), channel)
{
    _beatAnimatedProperty.setStartValue(QVariant(_boundingRect.height()/2));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);
}

void MinoAnimationWaveform::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    QColor color(Qt::green);
    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    for (int i=0;i<_boundingRect.width();i++)
    {
        int currentAnimatedPropertyValue = 1+(qrand()%_beatAnimatedProperty.currentValue().toInt());
        _itemGroup.addToGroup(_scene->addLine(i,((_boundingRect.height()/2)-currentAnimatedPropertyValue),i,((_boundingRect.height()/2)+currentAnimatedPropertyValue),QPen(color)));
    }

}
