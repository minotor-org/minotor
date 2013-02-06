#include "minoprogram.h"

#include "minotor.h"

#include <QBrush>
#include <QDebug>

MinoProgram::MinoProgram(Minotor *minotor) :
    QObject(minotor),
    _image(NULL)
{
    _scene = minotor->scene();

    _scene->addItem(&_itemGroup);

    minotor->addProgram(this);
}

MinoProgram::~MinoProgram()
{
    foreach (MinoAnimation *animation, _minoAnimations)
    {
        delete(animation);
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

void MinoProgram::addAnimation(MinoAnimation *animation)
{
    // Add animation to program's list
    _minoAnimations.append(animation);

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
}

MinoAnimation* MinoProgram::addAnimation(const QString animationClassName)
{
    MinoAnimation *animation = MinoAnimationFactory::createObject(animationClassName.toAscii(), this);
    if(animation)
    {
        addAnimation(animation);
    }
    return animation;
}

void MinoProgram::setDrawingPos(const QPointF pos)
{
    qDebug() << "draw pos" << pos;
     _itemGroup.setPos(pos);
     _drawingPos = pos;
}

void MinoProgram::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    // Set position to origin
    _itemGroup.setPos(0,0);

    if(ppqn==0)
    {
        if(_minoAnimationsToEnable.count())
        {
            foreach(MinoAnimation *animation, _minoAnimationsToEnable)
            {
                animation->graphicItem()->show();
                qDebug() << "animation: (0,0) scene: " << animation->graphicItem()->mapToScene(0, 0);
                animation->_setEnabled(true);
            }
            _minoAnimationsToEnable.clear();
        }
        if(_minoAnimationsToDisable.count())
        {
            qDebug() << "program" << _id << ": (0,0) scene: " << _itemGroup.mapToScene(0,0);
            foreach(MinoAnimation *animation, _minoAnimationsToDisable)
            {
                animation->graphicItem()->hide();
                qDebug() << "animation: (0,0) scene: " << animation->graphicItem()->mapToScene(0, 0);
                animation->_setEnabled(false);
            }
            _minoAnimationsToDisable.clear();
        }
    }

    // Animate whole content (ie. this includes objects creation/desctruction moves)
    foreach(MinoAnimation *minoAnimation, _minoAnimations)
    {
        //if(minoAnimation->enabled())
        {
            minoAnimation->animate(gppqn, ppqn, qn);
        }
    }

    // Reset position to the affected one_itemGroup
    _itemGroup.setPos(_drawingPos);
    if(ppqn==0) qDebug() << "program" << _id << ": (0,0) scene: " << _itemGroup.mapToScene(0,0) << "drawing pos:" << _drawingPos;

    // Set background
    _image->fill(Qt::black);

    // Render the scene at previously saved view rect
    QPainter painter(_image);
    _scene->render(&painter, QRectF(_image->rect()), QRectF(_drawingPos, _rect.size()), Qt::IgnoreAspectRatio);

    // Let's connected object to know the program's animation is done
    emit animated();
}

void MinoProgram::destroyAnimation(QObject *animation)
{
    _minoAnimations.removeAt(_minoAnimations.indexOf(static_cast<MinoAnimation*>(animation)));
}

Minotor *MinoProgram::minotor()
{
     return static_cast<Minotor*>(parent());
}

void MinoProgram::registerAnimationEnableChange(MinoAnimation *animation, const bool on)
{
    if(on)
    {
        _minoAnimationsToEnable.append(animation);
    }
    else
    {
        _minoAnimationsToDisable.append(animation);
    }
}
