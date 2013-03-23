#include "minoprogram.h"

#include "minotor.h"
#include "minoanimationgroup.h"

#include <QBrush>
#include <QDebug>

MinoProgram::MinoProgram(Minotor *minotor) :
    QObject(minotor),
    _image(NULL),
    _onAir(false)
{
    _scene = minotor->scene();
    _scene->addItem(&_itemGroup);

    _beatFactor.setObjectName("Beat");
    _beatFactor.addItem("1", 24);
    _beatFactor.addItem("2", 48);
    _beatFactor.addItem("4", 96);
    _beatFactor.addItem("8", 192);
    _beatFactor.addItem("16", 384);
    _beatFactor.setCurrentItem("1");
    _properties.append(&_beatFactor);

    minotor->addProgram(this);
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

    const unsigned int beat = _beatFactor.currentItem()->real();
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
        if(animationGroup->enabled())
        {
            foreach(MinoAnimation *animation, animationGroup->animations())
            {
                if(animation->enabled())
                {
                    animation->animate(uppqn, gppqn, ppqn, qn);
                }
            }
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
    _animationGroups.removeAt(_animationGroups.indexOf(static_cast<MinoAnimationGroup*>(group)));
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
     return static_cast<Minotor*>(parent());
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
    _animationGroups.append(group);
    _itemGroup.addToGroup(group->itemGroup());
    group->itemGroup()->setParentItem(&_itemGroup);
    group->itemGroup()->setGroup(&_itemGroup);
    group->itemGroup()->setPos(0,0);
    connect(group, SIGNAL(updated()), this, SIGNAL(updated()));
    emit updated();
}

void MinoProgram::moveAnimation(MinoAnimationGroup *srcGroup, int srcAnimationId, MinoAnimationGroup *destGroup, int destAnimationId)
{
    if(srcGroup==destGroup)
    {
        if(srcAnimationId!=destAnimationId)
            srcGroup->moveAnimation(srcAnimationId, destAnimationId);
    } else {
        MinoAnimation *animation = srcGroup->takeAnimationAt(srcAnimationId);
        qDebug() << "MinoProgram> animation(" << srcAnimationId << "):" << animation;
        destGroup->insertAnimation(animation, destAnimationId);
        qDebug() << "MinoProgram> animation inserted";
    }
}
