#include "minastars.h"

MinaStars::MinaStars(QObject *object) :
    MinoAnimation(object)
{
    _ecrPosition.setStartValue(0.0);
    _ecrPosition.setEndValue(2.0);
    _ecrPosition.setEasingCurve(QEasingCurve::InQuart);

    _beatDuration = new MinoItemizedProperty(this);
    _beatDuration->setObjectName("duration");
    _beatDuration->setLabel("Duration");
    _beatDuration->addItem("1/4", 6);
    _beatDuration->addItem("1/2", 12);
    _beatDuration->addItem("1", 24);
    _beatDuration->addItem("2", 48);
    _beatDuration->addItem("4", 96);
    _beatDuration->addItem("8", 192);
    _beatDuration->addItem("16", 384);
    _beatDuration->setCurrentItem("1");
    _beatDuration->setLinear();

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");

    _generatorDensity = new MinoItemizedProperty(this);
    _generatorDensity->setObjectName("density");
    _generatorDensity->setLabel("Density");
    _generatorDensity->addItem("1", 1);
    _generatorDensity->addItem("2", 2);
    _generatorDensity->addItem("3", 3);
    _generatorDensity->addItem("4", 4);
    _generatorDensity->addItem("5", 5);
    _generatorDensity->addItem("6", 6);
    _generatorDensity->addItem("7", 7);
    _generatorDensity->addItem("8", 8);
    _generatorDensity->setCurrentItem("1");
    _generatorDensity->setLinear();

}

void MinaStars::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    QColor color = _color->color();
    _ecrPosition.setEasingCurve(_generatorCurve->easingCurveType());

    const unsigned int b = _beatFactor->loopSizeInPpqn();
    const unsigned int density = _generatorDensity->currentItem()->real();
    const unsigned int duration = _beatDuration->currentItem()->real();

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
            const qreal progress = item.progressForUppqn(uppqn);
            item._graphicsItem->setScale(_ecrPosition.valueForProgress(progress));
            item._graphicsItem->setOpacity((_ecrPosition.valueForProgress(progress)/2));
        }
    }
}

