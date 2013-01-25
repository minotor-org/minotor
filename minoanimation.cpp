#include "minoanimation.h"
#include "minochannel.h"

MinoAnimation::MinoAnimation(QString name, MinoChannel *parent) :
    QObject(parent),
    _name(name)
{
    setChannel(parent);

    _beatFactor.setObjectName("Beat factor");
    _properties.append(&_beatFactor);
    _beatFactor.setValue(0.4); // BeatFactor 1
}

void MinoAnimation::setChannel(MinoChannel *channel)
{
    _scene = channel->scene();
    _boundingRect = channel->boundingRect();
    channel->itemGroup()->addToGroup(&_itemGroup);
    _itemGroup.setParentItem(channel->itemGroup());
}

qreal MinoAnimation::ratioToBeatFactor(qreal value)
{
    QList<qreal> tempoList;
    tempoList.append(0.25);
    tempoList.append(0.5);
    tempoList.append(1);
    tempoList.append(2);
    tempoList.append(4);
    tempoList.append(8);
    tempoList.append(16);

    int step = (tempoList.count()-1) * value;
    //qDebug() << "value" << (qreal)value << "step" << step << "factor" << tempoList.at(step);
    return tempoList.at(step);
}

void MinoAnimation::computeAnimaBeatProperty(const unsigned int gppqn)
{
    const unsigned int ppqnMax = ratioToBeatFactor(_beatFactor.value()) * 24;
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);

    /*
    qDebug() << "computeAnimaBeatProperty"
             << "beatFactor" << ratioToBeatFactor(_beatFactor.value())
             << "durationFactor" << durationFactor;
    */
}

