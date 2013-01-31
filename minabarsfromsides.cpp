#include "minabarsfromsides.h"
#include <QColor>
#include <QDebug>

MinaBarsFromSides::MinaBarsFromSides(Minotor *minotor) :
    MinoAnimation(minotor)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);
}

void MinaBarsFromSides::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
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
            qreal lineLength = qrandF() * _beatAnimatedProperty.currentValue().toReal() * _boundingRect.width() / 2.0;
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.left(), i, _boundingRect.left()+lineLength, i, QPen(color)));
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.width()-lineLength, i, _boundingRect.width(), i, QPen(color)));
        }
    }
    else
    {
        for (int i=0;i< _boundingRect.width();i++)
        {
            qreal lineLength = qrandF() * _beatAnimatedProperty.currentValue().toReal() * _boundingRect.height() / 2.0;
            _itemGroup.addToGroup(_scene->addLine(i, _boundingRect.top(), i, _boundingRect.top()+lineLength, QPen(color)));
            _itemGroup.addToGroup(_scene->addLine(i, (qreal)_boundingRect.height()-lineLength, i, _boundingRect.height(), QPen(color)));
        }
    }
}
