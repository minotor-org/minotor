#include "minaflash.h"

#include <QColor>
#include <QDebug>

MinaFlash::MinaFlash(MinoAnimationGroup *group):
    MinoAnimation(group),
    _rectItem(NULL)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

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
    computeAnimaBeatProperty(gppqn);

    QColor color = _color->color();
    _rectItem->setBrush(QBrush(color));
    _rectItem->setOpacity(_beatAnimatedProperty.currentValue().toFloat());
}
