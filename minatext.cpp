#include "minatext.h"
#include <QLabel>

MinaText::MinaText(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.01));

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

    _text = new MinoPropertyText(this);

    _generatorStyle = new MinoItemizedProperty(this);
    _generatorStyle->setObjectName("style");
    _generatorStyle->setLabel("Style");
    _generatorStyle->addItem("P:F T:F", 0);
    _generatorStyle->addItem("P:R T:F", 1);
    _generatorStyle->addItem("P:F T:R", 2);
    _generatorStyle->addItem("P:R=T:R", 3);
    _generatorStyle->addItem("P:R T:R", 4);
    _generatorStyle->setCurrentItem("P:R T:F");
}

void MinaText::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    QColor color = _color->color();

    const unsigned int b = _beatFactor->loopSizeInPpqn();
    if ((gppqn%b)==0)
    {
        QGraphicsTextItem* item = _scene->addText(_text->text(),QFont("Arial",12,QFont::Bold,false));
        QRectF tRect = item->boundingRect();
        tRect.adjust(0,0,-1,-1);
        tRect.moveCenter(_boundingRect.center());
        item->setPos(tRect.topLeft());
        item->setDefaultTextColor(color);

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
            item->setTransformOriginPoint(randPoint-_boundingRect.center());
        }
            break;
        case 4:
        {
            item->setPos(qrandPointF()-_boundingRect.center());
            item->setTransformOriginPoint(qrandPointF()-_boundingRect.center());
        }
            break;
        }

        const unsigned int duration = _beatDuration->currentItem()->real();
        MinoAnimatedItem maItem (uppqn, duration, item);
        _itemGroup.addToGroup(item);
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

            _animatedItems.at(i)._graphicsItem->setScale(_beatAnimatedProperty.currentValue().toReal());
        }
    }
}

