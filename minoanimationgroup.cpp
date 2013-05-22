#include "minoanimationgroup.h"

#include <QDebug>

#include "minoprogram.h"
#include "minopersistentobjectfactory.h"

MinoAnimationGroup::MinoAnimationGroup(QObject *parent) :
    MinoPersistentObject(parent),
    _enabled(false),
    _program(NULL)
{
    Q_ASSERT(parent);
    _program = qobject_cast<MinoProgram*>(parent);
    Q_ASSERT(_program);
    connect(this, SIGNAL(destroyed(QObject*)), _program, SLOT(destroyGroup(QObject*)));
}

int MinoAnimationGroup::id ()
{
    return _program->animationGroups().indexOf(this);
}

void MinoAnimationGroup::addAnimation(MinoAnimation *animation)
{
    insertAnimation(animation);
}

MinoAnimationGroup::~MinoAnimationGroup()
{
    foreach (MinoAnimation *animation, _animations)
    {
        delete animation;
    }
}

MinoAnimation* MinoAnimationGroup::addAnimation(const QString animationClassName, int index)
{

    MinoAnimation *animation = qobject_cast<MinoAnimation*>(MinoPersistentObjectFactory::createObject(animationClassName.toAscii(), this));
    if(animation)
    {
        insertAnimation(animation, index);
    }
    return animation;
}

void MinoAnimationGroup::insertAnimation(MinoAnimation *animation, int index)
{
    qDebug() << Q_FUNC_INFO
             << "animation:" << animation
             << "index:" << index;
    // Add animation to program's list
    if(index<0)
        index = _animations.size();
    _animations.insert(index, animation);

    // Will remove animation from list when destroyed
    connect(animation, SIGNAL(destroyed(QObject*)), this, SLOT(destroyAnimation(QObject*)));

    // Add to program QGraphicsItemGroup to ease group manipulation (ie. change position, brightness, etc.)
    _itemGroup.addToGroup(animation->graphicItem());

    // Re-parent animation to our itemGroup
    animation->graphicItem()->setVisible(this->_enabled);
    animation->graphicItem()->setParentItem(&_itemGroup);
    animation->graphicItem()->setGroup(&_itemGroup);
    animation->graphicItem()->setPos(0,0);

    // Reorder Z values
    for(int z=0; z<_animations.count(); z++)
    {
        _animations.at(z)->graphicItem()->setZValue(z);
    }
    animation->setGroup(this);
    emit animationAdded(animation);
}

void MinoAnimationGroup::moveAnimation(int srcIndex, int destIndex, MinoAnimationGroup *destGroup)
{
    qDebug() << Q_FUNC_INFO
             << "this:" << this
             << "destIndex:" << destIndex
             << "destGroup:" << destGroup;

    if(destGroup == NULL)
        destGroup = this;

    //Position -1 is used to place the item at the end of the list
    if (destIndex == -1)
    {
        destIndex = destGroup->animations().count()-1;
    }

    if((this==destGroup) && (srcIndex==destIndex))
    {
        // Nothing to do: destination is the same as source
    }
    else if(this==destGroup)
    {
        // Destination group is our group
        _animations.move(srcIndex, destIndex);
        for (int z=0;z<_animations.count();z++)
        {
            _animations.at(z)->graphicItem()->setZValue(z);
        }
        emit animationMoved(_animations.at(destIndex));
    }
    else
    {
        // Destination group is not this group
        // Let's take animation from this group
        MinoAnimation *animation = takeAnimationAt(srcIndex);
        // Prevent group from emitting animationAdded signal
        destGroup->blockSignals(true);
        destGroup->insertAnimation(animation, destIndex);
        destGroup->blockSignals(false);
        animation->setEnabled(destGroup->enabled());
        emit animationMoved(animation);
    }
}

MinoAnimation* MinoAnimationGroup::takeAnimationAt(int index)
{
    MinoAnimation *animation = _animations.takeAt(index);
    disconnect(animation);
    animation->setGroup(NULL);
    if (_animations.count() == 0)
    {
        this->deleteLater();
    }
    return animation;
}

void MinoAnimationGroup::setDelayedEnabled(const bool on)
{
    _program->registerAnimationGroupEnableChange(this, on);
}

void MinoAnimationGroup::setEnabled(const bool on)
{
    if(on != _enabled)
    {
        _setEnabled(on);
    }
}

void MinoAnimationGroup::_setEnabled(const bool on)
{
    _enabled = on;
    foreach(MinoAnimation *animation, _animations)
    {
        animation->_setEnabled(on);
    }

    _itemGroup.setVisible(on);
    emit enabledChanged(on);
}

void MinoAnimationGroup::destroyAnimation(QObject *animation)
{
    _animations.removeAt(_animations.indexOf(static_cast<MinoAnimation*>(animation)));
    if (_animations.count() == 0)
    {
        this->deleteLater();
    }
}

