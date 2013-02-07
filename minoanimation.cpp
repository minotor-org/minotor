#include "minoanimation.h"

#include "minotor.h"
#include "minoprogram.h"

MinoAnimation::MinoAnimation(MinoProgram *program) :
    QObject(program),
    _program(program),
    _enabled(true)
{
    _scene = program->minotor()->scene();
    _boundingRect = program->minotor()->displayRect();
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

void MinoAnimation::setDelayedEnabled(const bool on)
{
        _program->registerAnimationEnableChange(this, on);
}

void MinoAnimation::setEnabled(const bool on)
{
    if(on != _enabled)
    {
        _setEnabled(on);
    }
}

void MinoAnimation::_setEnabled(const bool on)
{
    _enabled = on;
    graphicItem()->setVisible(on);
    emit enabledChanged(on);
}
