#include "minoanimation.h"

#include "minotor.h"
#include "minoprogram.h"
#include "minoanimationgroup.h"

MinoAnimation::MinoAnimation(QObject *parent) :
    MinoPersistentObject(parent),
    _group(NULL),
    _enabled(false)
{
    Q_ASSERT(parent);
    if(MinoAnimationGroup* mag = qobject_cast<MinoAnimationGroup*>(parent))
    {
        setGroup(mag);
        _scene = Minotor::minotor()->scene();
        _boundingRect = Minotor::minotor()->displayRect();
    }
    _color = new MinoPropertyColor(this);
    QColor randColor;
    randColor.setHslF(qrandF(), 1.0, 0.5);
    _color->setColor(randColor);

    _beatFactor = new MinoPropertyBeat(this);
}

int MinoAnimation::id()
{
    return _group->animations().indexOf(this);
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
           connect(this, SIGNAL(destroyed(QObject*)), group, SLOT(destroyAnimation(QObject*)));
        } else {
            graphicItem()->setVisible(false);
        }
        _group = group;
    }
}

void MinoAnimation::setEnabled(const bool on)
{
    if(on != _enabled)
    {
        _enabled = on;
        graphicItem()->setVisible(on);
        emit enabledChanged(on);
    }
}
