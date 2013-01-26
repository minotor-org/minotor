#include "minoanimationplasma.h"
#include <QColor>
#include <QGraphicsScene>
#include <QDebug>
#include <math.h>


MinoAnimationPlasma::MinoAnimationPlasma(MinoChannel *channel):
    MinoAnimation(QString("Bars from sides"), channel)
{
    _beatAnimatedProperty.setStartValue(QVariant(100));
    _beatAnimatedProperty.setEndValue(QVariant(10));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);

    for (int i=0;i<160;i+=10)
    {
        for (int j=0; j<240;j+=10)
        {
            _itemGroup.addToGroup(_scene->addLine ( j, i, j+1, i+1, QPen(Qt::red) ));
        }
    }
}

void MinoAnimationPlasma::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    QColor color(Qt::green);
    color.setRed(_color.value()*255);
    color.setBlue(_color.value()*255);
    color.setAlpha(_beatAnimatedProperty.currentValue().toInt());

    QColor transparency;
    transparency.setAlpha(0);

    foreach (QGraphicsItem *item , _itemGroup.childItems())
    {
        static_cast<QGraphicsLineItem*>(item)->setPen(QPen(transparency));
        qDebug() << "set transparency";
    }

    const qreal pixelCount = 24*16;
    for(int i=0; i<pixelCount; i++)
    {
            qDebug() << "set new color";
            int sinColor = int(128.0 + (128.0 * sin(i / 8.0)));
            color.setBlue(sinColor);
            // qDebug() << "x" << x << "y" << y;
            static_cast<QGraphicsLineItem*>(_itemGroup.childItems().at(i))->setPen(QPen(color));
            //_itemGroup.addToGroup(_scene->addLine( x, y, x+1, y+1, QPen(Qt::green)));

    }
}
