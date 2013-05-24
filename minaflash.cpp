#include "minaflash.h"

#include <QColor>
#include <QDebug>

MinaFlash::MinaFlash(QObject *object):
    MinoAnimation(object),
    _rectItem(NULL)
{
    _ecrOpacity.setEasingCurve(QEasingCurve::OutBounce);
    _ecrOpacity.setStartValue(1.0);
    _ecrOpacity.setEndValue(0.0);
    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    QColor color;
    color.setHsvF(0.4, 1.0, 1.0);
    _rectItem = _scene->addRect(_boundingRect, QPen(Qt::NoPen),QBrush(color));
    _rectItem->setVisible(false);
}

MinaFlash::~MinaFlash()
{
    //delete _rectItem;
}

void MinaFlash::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    QColor color = _color->color();

    _ecrOpacity.setEasingCurve(_generatorCurve->easingCurveType());

    _rectItem->setBrush(QBrush(color));
    _rectItem->setOpacity(_ecrOpacity.valueForProgress(_beatFactor->progressForGppqn(gppqn)));
}
