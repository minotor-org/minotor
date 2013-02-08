#include "minaexpandingobjects.h"

#include <QDebug>

MinaExpandingObjects::MinaExpandingObjects(MinoProgram *program):
    MinoAnimation(program)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.01));

    _beatDuration.setObjectName("Duration");
    _beatDuration.addItem("1/4", 6);
    _beatDuration.addItem("1/2", 12);
    _beatDuration.addItem("1", 24);
    _beatDuration.addItem("2", 48);
    _beatDuration.addItem("4", 96);
    _beatDuration.addItem("8", 192);
    _beatDuration.addItem("16", 384);
    _beatDuration.setCurrentItem("1");
    _properties.append(&_beatDuration);

    _color.setObjectName("Color");
    _properties.append(&_color);

    _itemGroup.setTransformOriginPoint(_boundingRect.center());
}

void MinaExpandingObjects::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    static QGraphicsEllipseItem* item = NULL;
    (void)qn;
   // computeAnimaBeatProperty(gppqn);

//    qDebug() << "uppqn" << uppqn;
    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);

    const unsigned int b = _beatFactor.currentItem()->real();
    if ((gppqn%b)==0)
    {
        QRect rect(_boundingRect);
        rect.adjust(0,0,-1,-1);
        item = _scene->addEllipse(rect, QPen(color), QBrush(Qt::NoBrush));
        item->setTransformOriginPoint(_boundingRect.center());
        // Be sure scale factor is 1.0 when we draw new items
        const unsigned int duration = _beatDuration.currentItem()->real();
        MinoAnimatedItem maItem (uppqn, duration, item);
        _itemGroup.addToGroup(item);
        _animatedItems.append(maItem);
        //            _itemGroup.setScale(1.0);
        qDebug() << "MinaExpandingObjects: add new item" << (_animatedItems.count()-1) << ": uppqn" << uppqn << "duration:" << duration;
    }
    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        if (uppqn > (item._startUppqn+item._duration))
        {
            qDebug() << "MinaExpandingObjects: delete item" << i << ": start" << item._startUppqn << "duration:" << item._duration << "end" << (item._startUppqn+item._duration) << "uppqn" << uppqn << "currentpos" << (uppqn-item._startUppqn);
            delete item._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal durationFactor = (qreal)(uppqn - item._startUppqn) / item._duration;
            _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);
            qDebug() << "MinaExpandingObjects: animate item" << i << ": start" << item._startUppqn << "duration:" << item._duration << "end" << (item._startUppqn+item._duration) << "uppqn" << uppqn << "currentpos" << (uppqn-item._startUppqn) << "durationFactor" << durationFactor;

            _animatedItems.at(i)._graphicsItem->setScale(_beatAnimatedProperty.currentValue().toReal());
        }
    }
}
