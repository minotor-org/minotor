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

    _color = new MinoPropertyReal(this);
    _color->setObjectName("Color");
    _color->setPreferred();
    _color->setParent(this);
    _color->setValue(qrandF());
    _mplMain.append(_color);

    _beatFactor = new MinoItemizedProperty(this);
    _beatFactor->setObjectName("Freq.");
    _beatFactor->addItem("16", 384);
    _beatFactor->addItem("8", 192);
    _beatFactor->addItem("4", 96);
    _beatFactor->addItem("2", 48);
    _beatFactor->addItem("1", 24);
    _beatFactor->addItem("1/2", 12);
    _beatFactor->addItem("1/4", 6);
    _beatFactor->setCurrentItem("1");
    _beatFactor->setLinear();
    _beatFactor->setPreferred();
    _beatFactor->setParent(this);
    _mplMain.append(_beatFactor);

    _propertyGrouped.append(&_mplMain);
}

void MinoAnimation::computeAnimaBeatProperty(const unsigned int gppqn)
{
    const unsigned int ppqnMax = _beatFactor->currentItem()->real();
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
