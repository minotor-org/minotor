#include "minoanimation.h"

#include "minotor.h"

MinoAnimation::MinoAnimation(Minotor *minotor) :
    QObject(minotor)
{
    _scene = minotor->scene();
    _boundingRect = minotor->displayRect();

    _beatFactor.setObjectName("Beat");
    _beatFactor.addItem("16", 16);
    _beatFactor.addItem("8", 8);
    _beatFactor.addItem("4", 4);
    _beatFactor.addItem("2", 2);
    _beatFactor.addItem("1", 1);
    _beatFactor.addItem("1/2", 0.5);
    _beatFactor.addItem("1/4", 0.25);
    _beatFactor.setCurrentItem("1");

    _properties.append(&_beatFactor);
}

void MinoAnimation::computeAnimaBeatProperty(const unsigned int gppqn)
{
    const unsigned int ppqnMax = _beatFactor.currentItem()->real() * 24;
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);

    /*
    qDebug() << "computeAnimaBeatProperty"
             << "beatFactor" << ratioToBeatFactor(_beatFactor.value())
             << "durationFactor" << durationFactor;
    */
}

