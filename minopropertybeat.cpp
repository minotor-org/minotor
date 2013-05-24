#include "minopropertybeat.h"

#include <QDebug>

MinoPropertyBeat::MinoPropertyBeat(QObject *parent) :
    MinoProperty(parent)
{
    _mcl = new MidiControllableList(this);
    setObjectName("loop-size");
    _mcl->setLabel("Loop");
    _mcl->setLinear();

    _mcl->addItem("16", 384);
    _mcl->addItem("8", 192);
    _mcl->addItem("4", 96);
    _mcl->addItem("2", 48);
    _mcl->addItem("1", 24);
    _mcl->addItem("1/2", 12);
    _mcl->addItem("1/4", 6);
    _mcl->setCurrentItem("1");

    connect(_mcl, SIGNAL(itemChanged(QString)), this, SIGNAL(loopSizeChanged(QString)));
}

qreal MinoPropertyBeat::progressForGppqn(const unsigned int gppqn) const
{
    const unsigned int ppqnMax = loopSizeInPpqn();
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    return durationFactor;
}

void MinoPropertyBeat::setObjectName(const QString &name)
{
    _mcl->setObjectName(name);
    MinoProperty::setObjectName(name);
}

