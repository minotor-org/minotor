#include "minaexpandingobjects.h"

#include <QGraphicsBlurEffect>

#include <QDebug>

MinaExpandingObjects::MinaExpandingObjects(MinoAnimationGroup *group):
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.01));

    _beatDuration = new MinoItemizedProperty(this);
    _beatDuration->setObjectName("Duration");
    _beatDuration->addItem("1/4", 6);
    _beatDuration->addItem("1/2", 12);
    _beatDuration->addItem("1", 24);
    _beatDuration->addItem("2", 48);
    _beatDuration->addItem("4", 96);
    _beatDuration->addItem("8", 192);
    _beatDuration->addItem("16", 384);
    _beatDuration->setLinear();
    _beatDuration->setCurrentItem("1");
    _mplLine2.append(_beatDuration);

    _generatorStyle = new MinoItemizedProperty(this);
    _generatorStyle->setObjectName("Style");
    _generatorStyle->addItem("P:F T:F", 0);
    _generatorStyle->addItem("P:R T:F", 1);
    _generatorStyle->addItem("P:F T:R", 2);
    _generatorStyle->addItem("P:R=T:R", 3);
    _generatorStyle->addItem("P:R T:R", 4);
    _generatorStyle->setCurrentItem("P:R T:F");
    _mplLine2.append(_generatorStyle);

    _generatorShape = new MinoItemizedProperty(this);
    _generatorShape->setObjectName("Shape");
    _generatorShape->addItem("Ellipse", 0);
    _generatorShape->addItem("Rect", 1);
    _generatorShape->addItem("Circle", 2);
    _generatorShape->addItem("Square", 3);
    _generatorShape->setCurrentItem("Ellipse");
    _mplLine3.append(_generatorShape);

    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(1.1);
    _itemGroup.setGraphicsEffect(blur);
}

void MinaExpandingObjects::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    static QGraphicsItem* item = NULL;
    (void)qn;
    (void)ppqn;

    QColor color;
    color.setHsvF(_color->value(), 1.0, 1.0);

    const unsigned int b = _beatFactor->currentItem()->real();
    if ((gppqn%b)==0)
    {
        const unsigned int shape = _generatorShape->currentItem()->real();
        switch(shape)
        {
        case 0:
            // HACK ellipse draw is 1 pixel larger than needed
            item = _scene->addEllipse(_boundingRect.adjusted(0,0,-1,-1), QPen(color), QBrush(Qt::NoBrush));
            break;
        case 1:
        {
            // HACK rect draw is 1 pixel larger than needed
            item = _scene->addRect(_boundingRect.adjusted(0,0,-1,-1), QPen(color), QBrush(Qt::NoBrush));
        }
            break;
        case 2:
        {
            const qreal height = qMin(_boundingRect.adjusted(0,0,-1,-1).height(), _boundingRect.adjusted(0,0,-1,-1).width());
            QRectF square(0, 0, height, height);
            square.moveCenter(_boundingRect.center());
            // HACK circle draw is 1 pixel larger than needed
            item = _scene->addEllipse(square, QPen(color), QBrush(Qt::NoBrush));
        }
            break;
        case 3:
        {
            const qreal height = qMin(_boundingRect.adjusted(0,0,-1,-1).height(), _boundingRect.adjusted(0,0,-1,-1).width());
            QRectF square(0, 0, height, height);
            square.moveCenter(_boundingRect.center());
            // HACK square draw is 1 pixel larger than needed
            item = _scene->addRect(square, QPen(color), QBrush(Qt::NoBrush));
        }
            break;
        }

        const unsigned int style = _generatorStyle->currentItem()->real();
        switch(style)
        {
        case 0:
            item->setTransformOriginPoint(_boundingRect.center());
            break;
        case 1:
        {
            item->setTransformOriginPoint(_boundingRect.center());
            item->setPos(qrandPointF()-_boundingRect.center());
        }
            break;
        case 2:
        {
            item->setTransformOriginPoint(qrandPointF());
        }
            break;
        case 3:
        {
            const QPointF randPoint = qrandPointF();
            item->setPos(randPoint-_boundingRect.center());
            item->setTransformOriginPoint(randPoint);
        }
            break;
        case 4:
        {
            item->setPos(qrandPointF()-_boundingRect.center());
            item->setTransformOriginPoint(qrandPointF());
        }
            break;
        }

        const unsigned int duration = _beatDuration->currentItem()->real();
        MinoAnimatedItem maItem (uppqn, duration, item);
        _itemGroup.addToGroup(item);
        _animatedItems.append(maItem);
//        qDebug() << "MinaExpandingObjects: add new item" << (_animatedItems.count()-1) << ": uppqn" << uppqn << "duration:" << duration;
    }
    for (int i=_animatedItems.count()-1;i>-1;i--)
    {
        const MinoAnimatedItem item = _animatedItems.at(i);
        if (uppqn > (item._startUppqn+item._duration))
        {
//            qDebug() << "MinaExpandingObjects: delete item" << i << ": start" << item._startUppqn << "duration:" << item._duration << "end" << (item._startUppqn+item._duration) << "uppqn" << uppqn << "currentpos" << (uppqn-item._startUppqn);
            delete item._graphicsItem;
            _animatedItems.removeAt(i);
        }
        else
        {
            const qreal durationFactor = (qreal)(uppqn - item._startUppqn) / item._duration;
            _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);
//            qDebug() << "MinaExpandingObjects: animate item" << i << ": start" << item._startUppqn << "duration:" << item._duration << "end" << (item._startUppqn+item._duration) << "uppqn" << uppqn << "currentpos" << (uppqn-item._startUppqn) << "durationFactor" << durationFactor;

            _animatedItems.at(i)._graphicsItem->setScale(_beatAnimatedProperty.currentValue().toReal());
        }
    }
}
