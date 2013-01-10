#include "minoanimationrandompixels.h"

#include <QDebug>

MinoAnimationRandomPixels::MinoAnimationRandomPixels(QGraphicsScene *scene, QObject *parent) :
    MinoAnimation(QString("Random pixels"), scene, parent)

{
    _animatedProperty.setStartValue(QVariant(255));
    _animatedProperty.setEndValue(QVariant(0));
    _animatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _density.setObjectName("Density");
    _properties.append(&_color);
    _properties.append(&_density);

    //TODO: remove hardcoded value
    for (int i=0;i<160;i+=10)
    {
        for (int j=0; j<240;j+=10)
        {
            _itemGroup.addToGroup(_scene->addLine ( j, i, j+1, i+1, QPen(Qt::red) ));
        }
    }

}

void MinoAnimationRandomPixels::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    QColor color(Qt::green);


    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);
    color.setAlpha(_animatedProperty.currentValue().toInt());
    // qDebug() << color.alpha();

    QColor transparency;
    transparency.setAlpha(0);

    if ((ppqn%6) == 0)
    {
        /*
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }
        */

        foreach (QGraphicsItem *item , _itemGroup.childItems())
        {
            static_cast<QGraphicsLineItem*>(item)->setPen(QPen(transparency));
        }

        const qreal pixelCount = _density.value()*(24*16);
        for(int i=0; i<pixelCount; i++)
        {
            int x = (qrand()%24);
            int y = (qrand()%16);

            int pixelIndex = (y*24)+x;
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
