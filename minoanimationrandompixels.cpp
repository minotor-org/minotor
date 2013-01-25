#include "minoanimationrandompixels.h"

#include <QDebug>

MinoAnimationRandomPixels::MinoAnimationRandomPixels(MinoChannel *channel) :
    MinoAnimation(QString("Random pixels"), channel)

{
    _beatAnimatedProperty.setStartValue(QVariant(255));
    _beatAnimatedProperty.setEndValue(QVariant(0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);

    _density.setObjectName("Density");
    _properties.append(&_density);

    for (int i=0;i<_boundingRect.height();i++)
    {
        for (int j=0; j<_boundingRect.width();j++)
        {
            _itemGroup.addToGroup(_scene->addLine ( j, i, j+1, i+1, QPen(Qt::red) ));
        }
    }

}

void MinoAnimationRandomPixels::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color(Qt::green);
    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);
    color.setAlpha(_beatAnimatedProperty.currentValue().toInt());

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
            int x = (qrand()%(static_cast<int>(_boundingRect.width())));
            int y = (qrand()%(static_cast<int>(_boundingRect.height())));

            int pixelIndex = (y*_boundingRect.width())+x;
            // qDebug() << "x" << x << "y" << y;
            static_cast<QGraphicsLineItem*>(_itemGroup.childItems().at(pixelIndex))->setPen(QPen(color));
                    //[pixelIndex]);
            //_itemGroup.addToGroup(_scene->addLine ( x, y, x+1, y+1, QPen(color) ));
        }
    } /*else {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
            static_cast<QGraphicsLineItem*>(item)->setPen(QPen(color));
        }
    }*/

}
