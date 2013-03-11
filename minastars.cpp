#include "minastars.h"

MinaStars::MinaStars(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.0));
    _beatAnimatedProperty.setEndValue(QVariant(2.0));
    _beatAnimatedProperty.setEasingCurve(QEasingCurve::InQuart);

    _beatDuration.setObjectName("Duration");
    _beatDuration.addItem("1/4", 6);
    _beatDuration.addItem("1/2", 12);
    _beatDuration.addItem("1", 24);
    _beatDuration.addItem("2", 48);
    _beatDuration.addItem("4", 96);
    _beatDuration.addItem("8", 192);
    _beatDuration.addItem("16", 384);
    _beatDuration.setCurrentItem("1");
    _mplDensity.append(&_beatDuration);

    _generatorDensity.setObjectName("Density");
    _generatorDensity.addItem("1", 1);
    _generatorDensity.addItem("2", 2);
    _generatorDensity.addItem("3", 3);
    _generatorDensity.addItem("4", 4);
    _generatorDensity.addItem("5", 5);
    _generatorDensity.addItem("6", 6);
    _generatorDensity.addItem("7", 7);
    _generatorDensity.addItem("8", 8);
    _generatorDensity.setCurrentItem("1");
    _mplDensity.append(&_generatorDensity);

    _propertyGrouped.append(&_mplDensity);
}

void MinaStars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);

    const unsigned int b = _beatFactor.currentItem()->real();
    const unsigned int density = _generatorDensity.currentItem()->real();
    const unsigned int duration = _beatDuration.currentItem()->real();

    QGraphicsItem *item = NULL;
    if ((gppqn%b)==0)
    {
        const QPointF offset(0.1, 0.1);
        QGraphicsItemGroup *group = new QGraphicsItemGroup(&_itemGroup, _scene);

        for (unsigned int i=0;i<density;i++)
        {
            QPointF randPoint = qrandPointF();

            const QLineF line1(randPoint-offset, randPoint+offset);
            item = _scene->addLine(line1, QPen(color));
            group->addToGroup(item);
            // Mirror on X
            randPoint.setX(_boundingRect.width()-randPoint.x());
            const QLineF line2(randPoint-offset, randPoint+offset);
            item = _scene->addLine(line2, QPen(color));
            group->addToGroup(item);
            // Mirror on Y
            randPoint.setY(_boundingRect.height()-randPoint.y());
            const QLineF line3(randPoint-offset, randPoint+offset);
            item = _scene->addLine(line3, QPen(color));
            group->addToGroup(item);
            // Mirror on X
            randPoint.setX(_boundingRect.width()-randPoint.x());
            const QLineF line4(randPoint-offset, randPoint+offset);
            item = _scene->addLine(line4, QPen(color));
            group->addToGroup(item);
        }
        group->setTransformOriginPoint(_boundingRect.center());
        MinoAnimatedItem maItem (uppqn, duration, group);
        _animatedItems.append(maItem);
    }

    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        if (uppqn > (item._startUppqn+item._duration))
        {
            delete item._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal durationFactor = (qreal)(uppqn - item._startUppqn) / item._duration;
            _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);
            item._graphicsItem->setScale(_beatAnimatedProperty.currentValue().toReal());
            item._graphicsItem->setOpacity((_beatAnimatedProperty.currentValue().toReal()/2));
        }
    }
}

