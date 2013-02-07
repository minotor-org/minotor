#include "minarandompixels.h"

#include <QDebug>

MinaRandomPixels::MinaRandomPixels(MinoProgram *program) :
    MinoAnimation(program)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Hue");
    _properties.append(&_color);

    _density.setObjectName("Density");
    _properties.append(&_density);

    for (qreal i=0.0;i<_boundingRect.height();i+=1.0)
    {
        for (qreal j=0.0; j<_boundingRect.width();j+=1.0)
        {
            const qreal h = 0.1;
            _itemGroup.addToGroup(_scene->addLine ( j, i, j+h, i+h, QPen(Qt::red) ));
        }
    }
}

void MinaRandomPixels::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0, _beatAnimatedProperty.currentValue().toReal());

    QColor transparency;
    transparency.setAlpha(0);

    if ((ppqn%6) == 0)
    {
        foreach (QGraphicsItem *item , _itemGroup.childItems())
        {
            static_cast<QGraphicsLineItem*>(item)->setPen(QPen(transparency));
        }

        const qreal pixelCount = _density.value()*(_boundingRect.width()*_boundingRect.height());
        for(int i=0; i<pixelCount; i++)
        {
            int x = (qrandF()*_boundingRect.width());
            int y = (qrandF()*_boundingRect.height());

            int pixelIndex = (y*_boundingRect.width())+x;

            static_cast<QGraphicsLineItem*>(_itemGroup.childItems().at(pixelIndex))->setPen(QPen(color));
        }
    } /*else {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
            static_cast<QGraphicsLineItem*>(item)->setPen(QPen(color));
        }
    }*/

}
