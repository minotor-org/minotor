#include "minoanimationbarsfromsides.h"
#include <QColor>
#include <QDebug>

MinoAnimationBarsFromSides::MinoAnimationBarsFromSides(MinoChannel *channel) :
    MinoAnimation(QString("Bars from sides"), channel)
{
    _beatAnimatedProperty.setStartValue(QVariant(_boundingRect.height()/2));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);

}

void MinoAnimationBarsFromSides::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;

    computeAnimaBeatProperty(gppqn);

    QColor color(Qt::green);
    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);

    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    if (qn%2)
    {
        for (int i=0;i< _boundingRect.height();i++)
        {
            int currentAnimatedPropertyValue = 1+(qrand()%_beatAnimatedProperty.currentValue().toInt());
            _itemGroup.addToGroup(_scene->addLine(0,i,(currentAnimatedPropertyValue),i,QPen(color)));
            _itemGroup.addToGroup(_scene->addLine((_boundingRect.width()-currentAnimatedPropertyValue-1),i,(_boundingRect.width()-1),i,QPen(color)));
        }
    }
    else
    {
        for (int i=0;i< _boundingRect.width();i++)
        {
            int currentAnimatedPropertyValue = 1+(qrand()%_beatAnimatedProperty.currentValue().toInt());
            _itemGroup.addToGroup(_scene->addLine(i,0,i,(currentAnimatedPropertyValue),QPen(color)));
            _itemGroup.addToGroup(_scene->addLine(i,(_boundingRect.height()-currentAnimatedPropertyValue-1),i,(_boundingRect.height()-1),QPen(color)));
        }
    }
}
