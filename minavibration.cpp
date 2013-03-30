#include "minavibration.h"
#include <QDebug>
#include <QGraphicsBlurEffect>

MinaVibration::MinaVibration(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue((qreal)_boundingRect.height()/2.0);
    _beatAnimatedProperty.setEndValue(QVariant(1.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::OutBounce);

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(1.1);
    _itemGroup.setGraphicsEffect(blur);

    _segments = new MinoPropertyReal(this);
    _segments->setObjectName("segment");
    _segments->setValue(0.3);
}

void MinaVibration::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    // Colors
    QColor color;
    color.setHsvF(_color->value(), 1.0, 1.0);

    const qreal middle = (qreal)_boundingRect.height()/2;
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    qreal currentX = -1;
    qreal currentY = middle;

    while (currentX < _boundingRect.width())
    {
        int maxSegments = _boundingRect.width()/2;

        //line up
        qreal randY = middle + ((qrandF() * _beatAnimatedProperty.currentValue().toReal())*0.9);
        qreal randX = currentX + qrandF()* qreal(_boundingRect.width()/(1+(_segments->value()*maxSegments)));

        _itemGroup.addToGroup(_scene->addLine(currentX, currentY, randX, randY, QPen(color)));

        currentX = randX;
        currentY = randY;

        //line down
        randY = middle - (qrandF() * _beatAnimatedProperty.currentValue().toReal())*0.9;
        randX = currentX + (int)(qrandF()* qreal(_boundingRect.width()/(1+(_segments->value()*maxSegments))));

        _itemGroup.addToGroup(_scene->addLine(currentX, currentY, randX, randY, QPen(color)));

        currentX = randX;
        currentY = randY;
    }
}
