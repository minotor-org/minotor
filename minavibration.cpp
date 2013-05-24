#include "minavibration.h"
#include <QDebug>
#include <QGraphicsBlurEffect>

MinaVibration::MinaVibration(QObject *object) :
    MinoAnimation(object)
{
    _ecrBarLenght.setStartValue((qreal)_boundingRect.height()/2.0);
    _ecrBarLenght.setEndValue(1.0);
    _ecrBarLenght.setEasingCurve(QEasingCurve::OutBounce);

    _segments = new MinoPropertyReal(this);
    _segments->setObjectName("segment");
    _segments->setLabel("Segment");
    _segments->setValue(0.3);

    _generatorCurve = new MinoPropertyEasingCurve(this, true);
    _generatorCurve->setObjectName("curve");
    _generatorCurve->setLabel("Curve");
}

void MinaVibration::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    // Colors
    QColor color = _color->color();

    _ecrBarLenght.setEasingCurve(_generatorCurve->easingCurveType());

    const qreal middle = (qreal)_boundingRect.height()/2;
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    qreal currentX = -1;
    qreal currentY = middle;
    const int maxSegments = _boundingRect.width()/2;
    qreal barLenghtFactor = _ecrBarLenght.valueForProgress(_beatFactor->progressForGppqn(gppqn));

    while (currentX < _boundingRect.width())
    {
        //line up
        qreal randY = middle + (qrandF() * barLenghtFactor * 0.9);
        qreal randX = currentX + qrandF()* qreal(_boundingRect.width()/(1+(_segments->value()*maxSegments)));

        _itemGroup.addToGroup(_scene->addLine(currentX, currentY, randX, randY, QPen(color)));

        currentX = randX;
        currentY = randY;

        //line down
        randY = middle - (qrandF() * barLenghtFactor * 0.9);
        randX = currentX + (int)(qrandF()* qreal(_boundingRect.width()/(1+(_segments->value()*maxSegments))));

        _itemGroup.addToGroup(_scene->addLine(currentX, currentY, randX, randY, QPen(color)));

        currentX = randX;
        currentY = randY;
    }
}
