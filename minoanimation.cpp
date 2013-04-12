#include "minoanimation.h"

#include "minotor.h"
#include "minoprogram.h"
#include "minoanimationgroup.h"

MinoAnimation::MinoAnimation(MinoAnimationGroup *group) :
    MinoPersistentObject(group),
    _group(group),
    _enabled(true)
{
    connect(this, SIGNAL(destroyed(QObject*)), group, SLOT(destroyAnimation(QObject*)));
    _program = group->program();
    _scene = _program->minotor()->scene();
    _boundingRect = _program->minotor()->displayRect();

    _color = new MinoPropertyColor(this);
    QColor randColor;
    randColor.setHslF(qrandF(), 1.0, 0.5);
    _color->setColor(randColor);

    _beatFactor = new MinoPropertyBeat(this);

    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
}

void MinoAnimation::computeAnimaBeatProperty(const unsigned int gppqn)
{
    const unsigned int ppqnMax = _beatFactor->loopSizeInPpqn();
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);

    /*
    qDebug() << "computeAnimaBeatProperty"
             << "beatFactor" << ratioToBeatFactor(_beatFactor->value())
             << "durationFactor" << durationFactor;
    */
}

QPointF MinoAnimation::qrandPointF()
{
    return QPointF(qrandF()*_boundingRect.width(), qrandF()*_boundingRect.height());
}

void MinoAnimation::setGroup(MinoAnimationGroup *group)
{
    if(_group != group)
    {
        _group = group;
        setParent(group);
        if(_group) {
           _program = _group->program();
           emit groupChanged(_program->id(), _group->id());
        } else {
            _program = NULL;
            graphicItem()->setVisible(false);
        }
    }
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
