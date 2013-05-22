#include "minoanimation.h"

#include "minotor.h"
#include "minoprogram.h"
#include "minoanimationgroup.h"

MinoAnimation::MinoAnimation(QObject *parent) :
    MinoPersistentObject(parent),
    _group(NULL),
    _enabled(true)
{
    Q_ASSERT(parent);
    if(MinoAnimationGroup* mag = qobject_cast<MinoAnimationGroup*>(parent))
    {
        setGroup(mag);
        _scene = _program->minotor()->scene();
        _boundingRect = _program->minotor()->displayRect();
    }
    _color = new MinoPropertyColor(this);
    QColor randColor;
    randColor.setHslF(qrandF(), 1.0, 0.5);
    _color->setColor(randColor);

    _beatFactor = new MinoPropertyBeat(this);

    _beatAnimatedProperty.setStartValue(QVariant(1.0));
    _beatAnimatedProperty.setEndValue(QVariant(0.0));
}

int MinoAnimation::id()
{
    return _group->animations().indexOf(this);
}

void MinoAnimation::computeAnimaBeatProperty(const unsigned int gppqn)
{
    const unsigned int ppqnMax = _beatFactor->loopSizeInPpqn();
    const qreal lppqn = gppqn % ppqnMax;
    const qreal durationFactor = lppqn / ppqnMax;
    _beatAnimatedProperty.setCurrentTime(qreal(_beatAnimatedProperty.duration()) * durationFactor);
}

QPointF MinoAnimation::qrandPointF()
{
    return QPointF(qrandF()*_boundingRect.width(), qrandF()*_boundingRect.height());
}

void MinoAnimation::setGroup(MinoAnimationGroup *group)
{
    if(_group != group)
    {
        if(_group)
        {
            disconnect(this, SIGNAL(destroyed(QObject*)), _group, SLOT(destroyAnimation(QObject*)));
        }
        setParent(group);
        if(group) {
           _program = group->program();
           connect(this, SIGNAL(destroyed(QObject*)), group, SLOT(destroyAnimation(QObject*)));
           emit groupChanged(_program->id(), group->id());
        } else {
            _program = NULL;
            graphicItem()->setVisible(false);
        }
        _group = group;
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
