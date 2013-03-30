#include "minacurve.h"
#include <QDebug>
#include <QGraphicsBlurEffect>

MinaCurve::MinaCurve(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    _generatorCurve = new MinoItemizedProperty(this);
    _generatorCurve->setObjectName("Curve");
    _generatorCurve->addItem("Linear", 0);
    _generatorCurve->addItem("InOutQuart", 1);
    _generatorCurve->addItem("OutInCubic", 2);
    _generatorCurve->addItem("OutInBack", 3);
    _generatorCurve->setCurrentItem("Linear");
    _mplLine2.append(_generatorCurve);

    _generatorAccel = new MinoItemizedProperty(this);
    _generatorAccel->setObjectName("Accel.");
    _generatorAccel->addItem("Linear", 0);
    _generatorAccel->addItem("OutInBack", 1);
    _generatorAccel->addItem("InOutBounce", 2);
    _generatorAccel->addItem("InOutQuart", 3);
    _generatorAccel->setCurrentItem("Linear");
    _mplLine2.append(_generatorAccel);

    QColor color;
    color.setHsvF(0.4, 1.0, 1.0);
    for (int i=0;i<_boundingRect.width();i++)
    {
        _items.append(_scene->addLine(i,_boundingRect.height(),i,5,QPen(color)));
        _itemGroup.addToGroup(_items[i]);
    }

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(1.1);
    _itemGroup.setGraphicsEffect(blur);
    _itemGroup.setVisible(false);
}

MinaCurve::~MinaCurve()
{
    //delete _itemGroup;
}

void MinaCurve::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;
    computeAnimaBeatProperty(gppqn);

    QColor color;
    color.setHsvF(_color->value(), 1.0, 1.0);

    QEasingCurve easing;
    //Drawing curve
    switch ((int)_generatorCurve->currentItem()->real())
    {
    case 0 :
    {
        easing = QEasingCurve(QEasingCurve::Linear);
    }
        break;
    case 1 :
    {
        easing = QEasingCurve(QEasingCurve::InOutQuart);
        break;
    }
    case 2 :
    {
        easing = QEasingCurve(QEasingCurve::OutInCubic);
    }
        break;
    case 3 :
    {
        easing = QEasingCurve(QEasingCurve::OutInBack);
    }
        break;
    }

    //Animation curve
    switch ((int)_generatorAccel->currentItem()->real())
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

    for (int i=0;i<_boundingRect.width();i++)
    {
        qreal pos = (qreal)i/(qreal)_boundingRect.width();
        qreal curvepos;
        pos += (1.0-_beatAnimatedProperty.currentValue().toReal());
        if(pos>1.0) pos -= 1.0;
        if(pos<=0.5)
        {
            curvepos = pos*2.0;
        }
        else
        {
            curvepos = (0.5-(pos-0.5))*2.0;
        }
        qreal curvefactor = easing.valueForProgress(curvepos);
        _items[i]->setLine(i,_boundingRect.height()/2,i,(_boundingRect.height()*curvefactor));
        _items[i]->setPen(QPen(color));
    }
}

