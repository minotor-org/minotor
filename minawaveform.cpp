#include "minawaveform.h"

#include <QDebug>

MinaWaveform::MinaWaveform(MinoProgram *program) :
    MinoAnimation(program)
{
    _beatAnimatedProperty.setStartValue((qreal)_boundingRect.height()/2.0);
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _color.setObjectName("Color");
    _properties.append(&_color);
}

void MinaWaveform::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);

    QLinearGradient
       grad(0.0, 0.0, 0.0, 16.0) ;
    grad.setColorAt(0.0, Qt::red) ;
    grad.setColorAt(0.3, Qt::yellow) ;
    grad.setColorAt(0.45, Qt::green) ;
    grad.setColorAt(0.55, Qt::green) ;
    grad.setColorAt(0.7, Qt::yellow) ;
    grad.setColorAt(1, Qt::red) ;

    const qreal middle = (qreal)_boundingRect.height()/2;
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    for (int i=0; i<_boundingRect.width(); i++)
    {
        qreal randHeight = qrandF() * _beatAnimatedProperty.currentValue().toReal();
        _itemGroup.addToGroup(_scene->addLine(i, middle-randHeight, i, middle+randHeight, QPen(QBrush(grad),1)));
    }
}
