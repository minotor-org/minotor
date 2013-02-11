#include "minafallingobjects.h"
#include <QDebug>
MinaFallingObjects::MinaFallingObjects(MinoProgram *program) :
    MinoAnimation(program)
{
    _beatAnimatedProperty.setStartValue(QVariant(0.0));
    _beatAnimatedProperty.setEndValue(QVariant(1.0));

    //First line of properties
    _beatDuration.setObjectName("Duration");
    _beatDuration.addItem("1/4", 6);
    _beatDuration.addItem("1/2", 12);
    _beatDuration.addItem("1", 24);
    _beatDuration.addItem("2", 48);
    _beatDuration.addItem("4", 96);
    _beatDuration.addItem("8", 192);
    _beatDuration.addItem("16", 384);
    _beatDuration.setCurrentItem("1");
    _mplLine2.append(&_beatDuration);


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

    _mplLine2.append(&_generatorDensity);

    _propertyGrouped.append(&_mplLine2);


    //Second line of properties
    _generatorDirection.setObjectName("Direction");
    _generatorDirection.addItem("Droite", 0);
    _generatorDirection.addItem("Gauche", 1);
    _generatorDirection.addItem("Haut", 2);
    _generatorDirection.addItem("Bas", 3);
    _generatorDirection.setCurrentItem("Droite");
    _mplLine3.append(&_generatorDirection);

    _generatorLength.setObjectName("Length");
    _generatorLength.addItem("1", 1);
    _generatorLength.addItem("2", 2);
    _generatorLength.addItem("3", 3);
    _generatorLength.addItem("4", 4);
    _generatorLength.addItem("5", 5);
    _generatorLength.addItem("6", 6);
    _generatorLength.addItem("7", 7);
    _generatorLength.addItem("8", 8);
    _generatorLength.addItem("9", 9);
    _generatorLength.addItem("10", 10);
    _generatorLength.addItem("11", 11);
    _generatorLength.addItem("12", 12);
    _generatorLength.setCurrentItem("8");

    _mplLine3.append(&_generatorLength);

    _propertyGrouped.append(&_mplLine3);

}

void MinaFallingObjects::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)qn;
    (void)ppqn;

    QColor color;
    color.setHsvF(_color.value(), 1.0, 1.0);

    const unsigned int b = _beatFactor.currentItem()->real();
    const unsigned int direction = _generatorDirection.currentItem()->real();
    const unsigned int length = _generatorLength.currentItem()->real();
    const unsigned int density = _generatorDensity.currentItem()->real();
    const unsigned int duration = _beatDuration.currentItem()->real();
    QGraphicsItem *item = NULL;
    if ((gppqn%b)==0)
    {


        int randomPos;
        for (int i=0;i<density;i++)
        {
            switch(direction)
            {
            case 0:
            {
                //left to right
                QLinearGradient grad(0.0, 0.0, length, 0.0) ;
                grad.setColorAt(0.0, Qt::black) ;
                grad.setColorAt(1, color) ;
                randomPos = qrand()%_boundingRect.height();
                item = _scene->addLine(0, randomPos, length, randomPos, QPen(QBrush(grad),1));
            }
                break;
            case 1:
            {
                //right to left
                QLinearGradient grad(0.0, 0.0, length, 0.0) ;
                grad.setColorAt(0.0, color) ;
                grad.setColorAt(1, Qt::black) ;
                randomPos = qrand()%_boundingRect.height();
                item = _scene->addLine(0, randomPos, length, randomPos, QPen(QBrush(grad),1));
            }
                break;
            case 2:
            {
                //bottom to top
                QLinearGradient grad(0.0, 0.0, 0.0, length) ;
                grad.setColorAt(0.0, color) ;
                grad.setColorAt(1, Qt::black) ;
                randomPos = qrand()%_boundingRect.width();
                item = _scene->addLine(randomPos, 0, randomPos, length, QPen(QBrush(grad),1));
            }
                break;
            case 3:
            {
                //top to bottom
                QLinearGradient grad(0.0, 0.0, 0.0, length) ;
                grad.setColorAt(0.0, Qt::black) ;
                grad.setColorAt(1, color) ;
                randomPos = qrand()%_boundingRect.width();
                item = _scene->addLine(randomPos, 0, randomPos, length, QPen(QBrush(grad),1));
            }
                break;
            }

            MinoAnimatedItem maItem (uppqn, duration, item);
            _itemGroup.addToGroup(item);
            _animatedItems.append(maItem);
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
            switch(direction)
            {
            case 0:
            {
                //left to right
                _animatedItems.at(i)._graphicsItem->setPos((_beatAnimatedProperty.currentValue().toReal()*((qreal)_boundingRect.width()+(length*2))-length),item._graphicsItem->pos().y());
            }
                break;
            case 1:
            {
                //right to left
                _animatedItems.at(i)._graphicsItem->setPos(((1-_beatAnimatedProperty.currentValue().toReal())*((qreal)_boundingRect.width()+(length*2))-length),item._graphicsItem->pos().y());
                break;
            }
            case 2:
            {
                //bottom to top
                _animatedItems.at(i)._graphicsItem->setPos(item._graphicsItem->pos().x(),((1-_beatAnimatedProperty.currentValue().toReal())*((qreal)_boundingRect.height()+(length*2))-length));
                break;
            }
            case 3:
            {
                //top to bottom
                _animatedItems.at(i)._graphicsItem->setPos(item._graphicsItem->pos().x(),((_beatAnimatedProperty.currentValue().toReal())*((qreal)_boundingRect.height()+(length*2))-length));
                break;
            }
            }
        }
    }
}

