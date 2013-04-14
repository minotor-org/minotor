#include "minavibration.h"
#include <QDebug>
#include <QGraphicsBlurEffect>

MinaVibration::MinaVibration(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _barLenght.setStartValue((qreal)_boundingRect.height()/2.0);
    _barLenght.setEndValue(1.0);
    _barLenght.setEasingCurve(QEasingCurve::OutBounce);

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(1.1);
    _itemGroup.setGraphicsEffect(blur);

    _segments = new MinoPropertyReal(this);
    _segments->setObjectName("segment");
    _segments->setLabel("Segment");
    _segments->setValue(0.3);
}

void MinaVibration::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)ppqn;
    (void)qn;

    computeAnimaBeatProperty(gppqn);

    // Colors
    QColor color = _color->color();

    const qreal middle = (qreal)_boundingRect.height()/2;
    foreach(QGraphicsItem* item, _itemGroup.childItems ())
    {
       delete item;
    }
    qreal currentX = -1;
    qreal currentY = middle;
    const int maxSegments = _boundingRect.width()/2;
    qreal barLenghtFactor = _barLenght.valueForProgress(_beatFactor->progressForGppqn(gppqn));

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
