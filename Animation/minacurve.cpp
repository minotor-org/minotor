#include "minacurve.h"

#include <QDebug>
#include <QGraphicsBlurEffect>

MinaCurve::MinaCurve(QObject *object) :
    MinoAnimation(object)
{
    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    _generatorAccel = new MinoPropertyEasingCurve(this);
    _generatorAccel->setObjectName("acceleration");
    _generatorAccel->setLabel("Accel.");
    // Please note that curve have not been tested with all parameters:
    // Theses following lines are here to show how to add curve type to selector
    _generatorAccel->addEasingCurveType(QEasingCurve::Linear);
    _generatorAccel->addEasingCurveType(QEasingCurve::OutInBack);
    _generatorAccel->addEasingCurveType(QEasingCurve::InOutBounce);
    _generatorAccel->addEasingCurveType(QEasingCurve::InOutQuart);
    _generatorAccel->setEasingCurveType(QEasingCurve::Linear);

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

    QColor color = _color->color();

    //Drawing curve
    QEasingCurve ecDraw(_generatorCurve->easingCurveType());

    //Animation curve
    QEasingCurve ecAnimation(_generatorAccel->easingCurveType());

    for (int i=0;i<_boundingRect.width();i++)
    {
        qreal pos = (qreal)i/(qreal)_boundingRect.width();
        qreal curvepos;
        pos += ecAnimation.valueForProgress(_beatFactor->progressForGppqn(gppqn));
        if(pos>1.0) pos -= 1.0;
        if(pos<=0.5)
        {
            curvepos = pos*2.0;
        }
        else
        {
            curvepos = (0.5-(pos-0.5))*2.0;
        }
        qreal curvefactor = ecDraw.valueForProgress(curvepos);
        _items[i]->setLine(i,_boundingRect.height()/2,i,(_boundingRect.height()*curvefactor));
        _items[i]->setPen(QPen(color));
    }
}

