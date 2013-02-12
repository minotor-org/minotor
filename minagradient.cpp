#include "minagradient.h"
#include "minoprogram.h"
#include <QDebug>

MinaGradient::MinaGradient(MinoProgram *program):
    MinoAnimation(program)
{

    _beatAnimatedProperty.setStartValue(QVariant(0.0));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::InOutBounce);

    _generatorStyle.setObjectName("Style");
    _generatorStyle.addItem("Radial", 0);
    _generatorStyle.addItem("Conical", 1);
    _generatorStyle.setCurrentItem("Radial");
    _mplLine2.append(&_generatorStyle);

    _generatorCurve.setObjectName("Curve");
    _generatorCurve.addItem("Linear", 0);
    _generatorCurve.addItem("OutInBack", 1);
    _generatorCurve.addItem("InOutBounce", 2);
    _generatorCurve.addItem("InOutQuart", 3);
    _generatorCurve.setCurrentItem("Linear");
    _mplLine2 .append(&_generatorCurve);

    _propertyGrouped.append(&_mplLine2);

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);

    _rectItem = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
}

MinaGradient::~MinaGradient()
{
    delete _rectItem;
}

void MinaGradient::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);
    QPointF center = _boundingRect.center();
    center.setX(center.x()+1);
    QGradient grad;

    switch ((int)_generatorStyle.currentItem()->real())
    {
    case 0 :
    {
        center.setY(center.y()+1);
        grad = QRadialGradient(center,_boundingRect.height());
    }
        break;
    case 1 :
    {
        center.setY(-1);
        grad = QConicalGradient(center,_boundingRect.height());
    }
        break;
    }

    switch ((int)_generatorCurve.currentItem()->real())
    {
    case 0 :
    {
        _beatAnimatedProperty.setEasingCurve(QEasingCurve::Linear);
    }
        break;
    case 1 :
    {
        _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutInBack);
    }
        break;
    case 2 :
    {
        _beatAnimatedProperty.setEasingCurve(QEasingCurve::InOutBounce);
    }
        break;
    case 3 :
    {
        _beatAnimatedProperty.setEasingCurve(QEasingCurve::InOutQuart);
    }
        break;
    }

    qreal param1 = _beatAnimatedProperty.currentValue().toReal();
    grad.setColorAt(param1, Qt::black);

    qreal param2;
    if ((_beatAnimatedProperty.currentValue().toReal()+0.166)>1)
    {
        param2 = (_beatAnimatedProperty.currentValue().toReal()+0.166)-1.0;
    }
    else
    {
        param2 = (_beatAnimatedProperty.currentValue().toReal()+0.166);
    }
    grad.setColorAt(param2, color) ;

    qreal param3;
    if ((_beatAnimatedProperty.currentValue().toReal()+0.333)>1)
    {
        param3 = (_beatAnimatedProperty.currentValue().toReal()+0.333)-1.0;
    }
    else
    {
        param3 = (_beatAnimatedProperty.currentValue().toReal()+0.333);
    }
    grad.setColorAt(param3, Qt::black) ;

    qreal param4;
    if ((_beatAnimatedProperty.currentValue().toReal()+0.499)>1)
    {
        param4 = (_beatAnimatedProperty.currentValue().toReal()+0.499)-1.0;
    }
    else
    {
        param4 = (_beatAnimatedProperty.currentValue().toReal()+0.499);
    }
    grad.setColorAt(param4, color) ;

    qreal param5;
    if ((_beatAnimatedProperty.currentValue().toReal()+0.666)>1)
    {
        param5 = (_beatAnimatedProperty.currentValue().toReal()+0.666)-1.0;
    }
    else
    {
        param5 = (_beatAnimatedProperty.currentValue().toReal()+0.666);
    }
    grad.setColorAt(param5, Qt::black) ;

    qreal param6;
    if ((_beatAnimatedProperty.currentValue().toReal()+0.833)>1)
    {
        param6 = (_beatAnimatedProperty.currentValue().toReal()+0.833)-1.0;
    }
    else
    {
        param6 = (_beatAnimatedProperty.currentValue().toReal()+0.833);
    }
    grad.setColorAt(param6, color) ;

    qreal param7;
    if ((_beatAnimatedProperty.currentValue().toReal()+1.0)>1)
    {
        param7 = (_beatAnimatedProperty.currentValue().toReal()+1.0)-1.0;
    }
    else
    {
        param7 = (_beatAnimatedProperty.currentValue().toReal()+1.0);
    }
    grad.setColorAt(param7, Qt::black) ;

    _rectItem->setBrush(QBrush(grad));

}
