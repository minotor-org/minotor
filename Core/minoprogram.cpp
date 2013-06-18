/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "minoprogram.h"

#include "minotor.h"
#include "minoanimationgroup.h"

#include <QBrush>
#include <QDebug>

MinoProgram::MinoProgram(QObject *parent) :
    MinoPersistentObject(parent),
    _image(NULL),
    _onAir(false)
{
    // Beat factor used for delayed animation launch
    _beatFactor = new MidiControllableList(this);
    _beatFactor->setObjectName("beat-factor");
    _beatFactor->setLabel("Beat");
    _beatFactor->addItem("1", 24);
    _beatFactor->addItem("2", 48);
    _beatFactor->addItem("4", 96);
    _beatFactor->addItem("8", 192);
    _beatFactor->addItem("16", 384);
    _beatFactor->setCurrentItem("1");

    MinoProgramBank *programBank = qobject_cast<MinoProgramBank*>(parent);
    Q_ASSERT(programBank);
    _scene = programBank->minotor()->scene();
    _scene->addItem(&_itemGroup);
    programBank->addProgram(this);
}

MinoProgram::~MinoProgram()
{
    foreach(MinoAnimationGroup *group, animationGroups())
    {
        delete group;
    }

    delete _image;
}

void MinoProgram::setRect(const QRect rect)
{
    _heightForWidthRatio = (qreal)rect.size().height() / (qreal)rect.size().width();
    if (_image) delete _image;
    _image = new QImage(rect.size(), QImage::Format_RGB32);
    _rect = rect;
}

void MinoProgram::setDrawingPos(const QPointF pos)
{
     _itemGroup.setPos(pos);
     _drawingPos = pos;
}

void MinoProgram::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    // Set position to origin
    _itemGroup.setPos(0,0);

    const unsigned int beat = _beatFactor->currentItem()->real();
    if((gppqn%beat)==0)
    {
        if(_animationGroupsToEnable.count())
        {
            foreach(MinoAnimationGroup *group, _animationGroupsToEnable)
            {
                group->_setEnabled(true);
            }
            _animationGroupsToEnable.clear();
        }
        if(_animationGroupsToDisable.count())
        {
            foreach(MinoAnimationGroup *group, _animationGroupsToDisable)
            {
                group->_setEnabled(false);
            }
            _animationGroupsToDisable.clear();
        }
    }

    // Animate whole content (ie. this includes objects creation/desctruction moves)
    foreach(MinoAnimationGroup *animationGroup, _animationGroups)
    {
        if(animationGroup->isAlive())
        {
            animationGroup->animate(uppqn,gppqn,ppqn,qn);
        }
    }

    // Reset position to the affected one_itemGroup
    _itemGroup.setPos(_drawingPos);

    // Set background
    _image->fill(Qt::black);

    // Render the scene at previously saved view rect
    QPainter painter(_image);
    //painter.setRenderHint(QPainter::Antialiasing);
    _scene->render(&painter, QRectF(_image->rect()), QRectF(_drawingPos, _rect.size()), Qt::IgnoreAspectRatio);

    // Let's connected object to know the program's animation is done
    emit animated();
}

void MinoProgram::destroyGroup(QObject *group)
{
    MinoAnimationGroup * mag = static_cast<MinoAnimationGroup*>(group);
    Q_ASSERT(mag);
    Q_ASSERT(_animationGroups.contains(mag));
    _animationGroups.removeAt(_animationGroups.indexOf(mag));
    const int ate = _animationGroupsToEnable.indexOf(static_cast<MinoAnimationGroup*>(group));
    if (ate != -1)
    {
        _animationGroupsToEnable.removeAt(ate);
    }
    const int atd = _animationGroupsToDisable.indexOf(static_cast<MinoAnimationGroup*>(group));
    if (atd != -1)
    {
        _animationGroupsToDisable.removeAt(atd);
    }
}

Minotor *MinoProgram::minotor()
{
    MinoProgramBank * programBank = qobject_cast<MinoProgramBank*>(parent());
    return programBank->minotor();
}

void MinoProgram::registerAnimationGroupEnableChange(MinoAnimationGroup *group, const bool on)
{
    if(on)
    {
        _animationGroupsToEnable.append(group);
    }
    else
    {
        _animationGroupsToDisable.append(group);
    }
}

void MinoProgram::setOnAir(bool on)
{
    if(on != _onAir)
    {
        _onAir = on;
        emit onAir(on);
    }
}

void MinoProgram::addAnimationGroup(MinoAnimationGroup *group)
{
    insertAnimationGroup(group, -1);
}

MinoAnimationGroup* MinoProgram::takeAnimationGroupAt(int index)
{
    MinoAnimationGroup *animationGroup = _animationGroups.takeAt(index);
    disconnect(animationGroup);
    animationGroup->setProgram(NULL);
    return animationGroup;
}

void MinoProgram::insertAnimationGroup(MinoAnimationGroup *group, int index)
{
    // Add group to program's list
    if(index<0)
        index = _animationGroups.size();
    _animationGroups.insert(index, group);

    // Will remove animation group from list when destroyed
    connect(group, SIGNAL(destroyed(QObject*)), this, SLOT(destroyGroup(QObject*)));

    reorderAnimationGroups();
    group->setProgram(this);
    emit updated();
    emit animationGroupAdded(group);
}

void MinoProgram::reorderAnimationGroups()
{
    //Reorder Z values
    for(int z=0; z<_animationGroups.count(); z++)
    {
        MinoAnimationGroup * mag = _animationGroups.at(z);
        mag->setParent(NULL);
        mag->setParent(this);
        mag->itemGroup()->setZValue(z);
    }
}

void MinoProgram::moveAnimationGroup(int srcIndex, int destIndex, MinoProgram *destProgram)
{
    qDebug() << Q_FUNC_INFO
             << "this:" << this
             << "destIndex:" << destIndex
             << "destProgram:" << destProgram;

    if(destProgram == NULL)
        destProgram = this;

    //Position -1 is used to place the item at the end of the list
    if (destIndex == -1)
    {
        destIndex = destProgram->animationGroups().count();
        // If group comes from this program, the last will be count()-1
        if(this==destProgram)
            destIndex -= 1;
    }

    if((this==destProgram) && (srcIndex==destIndex))
    {
        // Nothing to do: destination is the same as source
    }
    else if(this==destProgram)
    {
        // Destination group is our group
        _animationGroups.move(srcIndex, destIndex);
        reorderAnimationGroups();
        emit animationGroupMoved(_animationGroups.at(destIndex));
    }
    else
    {
        // Destination program is not this program
        // Let's take group from this program
        MinoAnimationGroup *group = takeAnimationGroupAt(srcIndex);
        // Prevent group from emitting animationAdded signal
        destProgram->blockSignals(true);
        destProgram->insertAnimationGroup(group, destIndex);
        destProgram->blockSignals(false);
        emit animationGroupMoved(group);
    }
}
