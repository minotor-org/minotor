#include "minoanimationgroup.h"

#include <QDebug>

#include "minoprogram.h"
#include "minoanimationfactory.h"

MinoAnimationGroup::MinoAnimationGroup(MinoProgram *parent) :
    QObject(parent),
    _enabled(false),
    _program(parent)
{
    connect(this, SIGNAL(destroyed(QObject*)), parent, SLOT(destroyGroup(QObject*)));
}

void MinoAnimationGroup::addAnimation(MinoAnimation *animation)
{
    // Add animation to program's list
    _animations.append(animation);

    // Will remove animation from list when destroyed
    connect(animation, SIGNAL(destroyed(QObject*)), this, SLOT(destroyAnimation(QObject*)));

    // Set position to origin
    //_itemGroup.setPos(0,0);

    // Add to program QGraphicsItemGroup to ease group manipulation (ie. change position, brightness, etc.)
    _itemGroup.addToGroup(animation->graphicItem());

    // Re-parent animation to our itemGroup
    animation->graphicItem()->setParentItem(&_itemGroup);
    animation->graphicItem()->setGroup(&_itemGroup);
    animation->graphicItem()->setPos(0,0);
    animation->graphicItem()->setZValue(_animations.count()-1);
    animation->setEnabled(false);

    // Lets others know something is changed
    emit updated();
}

MinoAnimationGroup::~MinoAnimationGroup()
{
    qDebug() << "~MinoAnimationGroup>"
                << "animations count:" << _animations.count();

    foreach (MinoAnimation *animation, _animations)
    {
        delete(animation);
    }
}

MinoAnimation* MinoAnimationGroup::addAnimation(const QString animationClassName)
{
    MinoAnimation *animation = MinoAnimationFactory::createObject(animationClassName.toAscii(), this);
    if(animation)
    {
        addAnimation(animation);
    }
    return animation;
}

void MinoAnimationGroup::insertAnimation(MinoAnimation *animation, int index)
{
    // Add animation to program's list
    _animations.insert(index, animation);

    // Will remove animation from list when destroyed
    connect(animation, SIGNAL(destroyed(QObject*)), this, SLOT(destroyAnimation(QObject*)));

    // Add to program QGraphicsItemGroup to ease group manipulation (ie. change position, brightness, etc.)
    _itemGroup.addToGroup(animation->graphicItem());

    // Re-parent animation to our itemGroup
    animation->graphicItem()->setParentItem(&_itemGroup);
    animation->graphicItem()->setGroup(&_itemGroup);
    animation->graphicItem()->setPos(0,0);

    // Reorder Z values
    for(int z; z<_animations.count(); z++)
    {
        _animations.at(z)->graphicItem()->setZValue(z);
    }
    animation->setGroup(this);

    emit updated();
}

void MinoAnimationGroup::moveAnimation(int oldIndex, int newIndex)
{
    _animations.move(oldIndex, newIndex);
    for (int z=0;z<_animations.count();z++)
    {
        _animations.at(z)->graphicItem()->setZValue(z);
    }
    // Lets others know something is changed
    emit updated();
}

MinoAnimation* MinoAnimationGroup::takeAnimationAt(int index)
{
    MinoAnimation *animation = _animations.takeAt(index);
    animation->setGroup(NULL);
    disconnect(animation, SIGNAL(destroyed(QObject*)), this, SLOT(destroyAnimation(QObject*)));
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
}

