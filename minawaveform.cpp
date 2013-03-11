#include "minawaveform.h"

#include <QDebug>

MinaWaveform::MinaWaveform(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue((qreal)_boundingRect.height()/2.0);
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

}

void MinaWaveform::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    // Colors
    QColor color, colorMin, colorMax;
    color.setHsvF(_color.value(), 1.0, 1.0);

    qreal minValue = _color.value()-0.2; if(minValue<0.0) minValue += 1.0;
    colorMin.setHsvF(minValue, 1.0, 1.0);

    qreal maxValue = _color.value()+0.2; if(maxValue>1.0) maxValue -= 1.0;
    colorMax.setHsvF(maxValue, 1.0, 1.0);

    QLinearGradient grad(0.0, 0.0, 0.0, (qreal)_boundingRect.height()) ;
    grad.setColorAt(0.0, colorMin) ;
    grad.setColorAt(0.3, color) ;
    grad.setColorAt(0.45, colorMax) ;
    grad.setColorAt(0.55, colorMax) ;
    grad.setColorAt(0.7, color) ;
    grad.setColorAt(1, colorMin) ;

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
