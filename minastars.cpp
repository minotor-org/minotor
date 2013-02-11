#include "minastars.h"

MinaStars::MinaStars(MinoProgram *program) :
    MinoAnimation(program)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.0));
    _beatAnimatedProperty.setEndValue(QVariant(2.0));

    _beatDuration.setObjectName("Duration");
    _beatDuration.addItem("1/4", 6);
    _beatDuration.addItem("1/2", 12);
    _beatDuration.addItem("1", 24);
    _beatDuration.addItem("2", 48);
    _beatDuration.addItem("4", 96);
    _beatDuration.addItem("8", 192);
    _beatDuration.addItem("16", 384);
    _beatDuration.setCurrentItem("1");
    _mplMain.append(&_beatDuration);

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
    const unsigned int length = _generatorLength.currentItem()->real();
    const unsigned int density = _generatorDensity.currentItem()->real();
    const unsigned int duration = _beatDuration.currentItem()->real();
    QGraphicsItem *item = NULL;
    if ((gppqn%b)==0)
    {


        int randomPosX1, randomPosY1, randomPosX2, randomPosY2, randomPosX3, randomPosY3, randomPosX4, randomPosY4;


        for (int i=0;i<density;i++)
        {
                const qreal h = 0.1;
                randomPosX1 = qrand()%(_boundingRect.height()/2);
                randomPosY1 =qrand()%(_boundingRect.width()/2);
                //up left
                item = _scene->addLine(randomPosX1-h, randomPosY1-h, randomPosX1+h, randomPosY1+h, QPen(color));
                item->setTransformOriginPoint(_boundingRect.center().x(),_boundingRect.center().y());
                MinoAnimatedItem maItem1 (uppqn, duration, item);
                _itemGroup.addToGroup(item);
                _animatedItems.append(maItem1);
                //up right
                randomPosX2 = _boundingRect.width()-randomPosX1;
                randomPosY2 = randomPosY1;
                item = _scene->addLine(randomPosX2-h, randomPosY2-h, randomPosX2+h, randomPosY2+h, QPen(color));
                item->setTransformOriginPoint(_boundingRect.center().x()-2,_boundingRect.center().y());
                MinoAnimatedItem maItem2 (uppqn, duration, item);
                _itemGroup.addToGroup(item);
                _animatedItems.append(maItem2);
                //bottom left
                randomPosX3 = randomPosX1;
                randomPosY3 =_boundingRect.height()-randomPosY1;
                item = _scene->addLine(randomPosX3-h, randomPosY3-h, randomPosX3+h, randomPosY3+h, QPen(color));
                item->setTransformOriginPoint(_boundingRect.center().x(),_boundingRect.center().y()-2);
                MinoAnimatedItem maItem3 (uppqn, duration, item);
                _itemGroup.addToGroup(item);
                _animatedItems.append(maItem3);
                //bottom right
                randomPosX4 = _boundingRect.width()-randomPosX1;
                randomPosY4 =_boundingRect.height()-randomPosY1;
                item = _scene->addLine(randomPosX4-h, randomPosY4-h, randomPosX4+h, randomPosY4+h, QPen(color));
                item->setTransformOriginPoint(_boundingRect.center().x()-2,_boundingRect.center().y()-2);
                MinoAnimatedItem maItem4 (uppqn, duration, item);
                _itemGroup.addToGroup(item);
                _animatedItems.append(maItem4);


        }

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

