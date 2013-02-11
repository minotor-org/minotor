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

    _color.setObjectName("Color");
    _mplMain.append(&_color);

    _beatFactor.setObjectName("Freq.");
    _beatFactor.addItem("16", 384);
    _beatFactor.addItem("8", 192);
    _beatFactor.addItem("4", 96);
    _beatFactor.addItem("2", 48);
    _beatFactor.addItem("1", 24);
    _beatFactor.addItem("1/2", 12);
    _beatFactor.addItem("1/4", 6);
    _beatFactor.setCurrentItem("1");
    _mplMain.append(&_beatFactor);

    _propertyGrouped.append(&_mplMain);

}

void MinoAnimation::computeAnimaBeatProperty(const unsigned int gppqn)
{
    const unsigned int ppqnMax = _beatFactor.currentItem()->real();
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);

    /*
    qDebug() << "computeAnimaBeatProperty"
             << "beatFactor" << ratioToBeatFactor(_beatFactor.value())
             << "durationFactor" << durationFactor;
    */
}

QPointF MinoAnimation::qrandPointF()
{
    return QPointF(qrandF()*_boundingRect.width(), qrandF()*_boundingRect.height());
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
