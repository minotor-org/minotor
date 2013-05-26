#include "minoanimationgroup.h"

#include <QDebug>

#include "minoprogram.h"
#include "minopersistentobjectfactory.h"

MinoAnimationGroup::MinoAnimationGroup(QObject *parent) :
    MinoPersistentObject(parent),
    _enabled(false),
    _alive(false),
    _program(NULL)
{
    Q_ASSERT(parent);
    Q_ASSERT(qobject_cast<MinoProgram*>(parent));
}

int MinoAnimationGroup::id ()
{
    return _program->animationGroups().indexOf(this);
}

void MinoAnimationGroup::addAnimation(MinoAnimation *animation)
{
    insertAnimation(animation);
}

void MinoAnimationGroup::setProgram(MinoProgram *program)
{
    if(_program != program)
    {
        setParent(program);
        if(program) {
            _itemGroup.setParentItem(program->itemGroup());
            _itemGroup.setGroup(program->itemGroup());
            _itemGroup.setPos(0,0);
            _itemGroup.setVisible(true);
        } else {
            _itemGroup.setVisible(false);
        }
        _program = program;
    }
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
        animation->setEnabled(on);
    }

    // Show _itemGroup (on=true) but dont hide it (on=false):
    //     it will be shuted-down when all animations stopped running..
    if(on)
    {
        _alive = true;
        _itemGroup.setVisible(true);
    }
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

void MinoAnimationGroup::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    bool alive = false;
    foreach(MinoAnimation *ma, _animations)
    {
        if(ma->isAlive())
        {
            ma->animate(uppqn, gppqn, ppqn, qn);
            alive = true;
        }
    }

	if(!alive)
        _itemGroup.setVisible(false);

    _alive = alive;
}

