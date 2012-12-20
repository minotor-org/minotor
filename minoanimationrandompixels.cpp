#include "minoanimationrandompixels.h"

#include <QDebug>

MinoAnimationRandomPixels::MinoAnimationRandomPixels(Minotor *parent) :
    MinoAnimation(parent)
{
    _animatedProperty.setStartValue(QVariant(255));
    _animatedProperty.setEndValue(QVariant(0));
    _animatedProperty.setEasingCurve(QEasingCurve::OutBounce);
    connect(parent, SIGNAL(colorControlChanged(int)), this, SLOT(colorControlChanged(int)));
}

void MinoAnimationRandomPixels::colorControlChanged(int value)
{
    _color = value * 2;
}

void MinoAnimationRandomPixels::animate(const unsigned int ppqn)
{
    const int currentTime = (qreal(_animatedProperty.duration())) * (((qreal)ppqn) / 24.0);
    _animatedProperty.setCurrentTime(currentTime);
    QColor color(Qt::blue);
    color.setRed(_color);
    color.setAlpha(_animatedProperty.currentValue().toInt());
    qDebug() << color.alpha();

    if ((ppqn%6) == 0)
    {
        qreal rand;
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
           delete item;
        }

        for(int i=0; i<170; i++)
        {
            int x = (qrand()%24)*10;
            int y = (qrand()%16)*10;
            qDebug() << "x" << x << "y" << y;
            _itemGroup.addToGroup(((Minotor*)parent())->scene()->addLine ( x, y, x+1, y+1, QPen(color) ));
        }
    } else {
        foreach(QGraphicsItem* item, _itemGroup.childItems ())
        {
            ((QGraphicsLineItem*)item)->setPen(QPen(color));
        }
    }

}
