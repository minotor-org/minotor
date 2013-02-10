#include "minabarsfromsides.h"
#include <QColor>
#include <QDebug>

MinaBarsFromSides::MinaBarsFromSides(MinoProgram *program) :
    MinoAnimation(program)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);
}

void MinaBarsFromSides::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;

    computeAnimaBeatProperty(gppqn);

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);
    qreal maxValue = (_color.value()<0.8)?_color.value()+0.2:1.0;
    qreal minValue = (_color.value()>0.2)?_color.value()-0.2:0.0;
    QColor colorMax;
    QColor colorMin;

    colorMax.setHsvF(maxValue, 1.0, 1.0);
    colorMin.setHsvF(minValue, 1.0, 1.0);

    QLinearGradient
            gradH(0.0, 0.0, 0.0, (qreal)_boundingRect.height()) ;
    gradH.setColorAt(0.0, colorMax) ;
    gradH.setColorAt(0.3, color) ;
    gradH.setColorAt(0.45, colorMin) ;
    gradH.setColorAt(0.55, colorMin) ;
    gradH.setColorAt(0.7, color) ;
    gradH.setColorAt(1, colorMax) ;

    QLinearGradient
            gradV(0.0, 0.0, (qreal)_boundingRect.width(), 0.0) ;
    gradV.setColorAt(0.0, colorMax) ;
    gradV.setColorAt(0.3, color) ;
    gradV.setColorAt(0.45, colorMin) ;
    gradV.setColorAt(0.55, colorMin) ;
    gradV.setColorAt(0.7, color) ;
    gradV.setColorAt(1, colorMax) ;


    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    if (qn%2)
    {
        for (int i=0;i< _boundingRect.height();i++)
        {
            qreal lineLength = qrandF() * _beatAnimatedProperty.currentValue().toReal() * _boundingRect.width() / 2.0;
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.left(), i, _boundingRect.left()+lineLength, i, QPen(QBrush(gradV),1)));
            _itemGroup.addToGroup(_scene->addLine(_boundingRect.width()-lineLength, i, _boundingRect.width(), i, QPen(QBrush(gradV),1)));
        }
    }
    else
    {
        for (int i=0;i< _boundingRect.width();i++)
        {
            qreal lineLength = qrandF() * _beatAnimatedProperty.currentValue().toReal() * _boundingRect.height() / 2.0;
            _itemGroup.addToGroup(_scene->addLine(i, _boundingRect.top(), i, _boundingRect.top()+lineLength, QPen(QBrush(gradH),1)));
            _itemGroup.addToGroup(_scene->addLine(i, (qreal)_boundingRect.height()-lineLength, i, _boundingRect.height(), QPen(QBrush(gradH),1)));
        }
    }
}
