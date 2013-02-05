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

void MinoProgram::setDrawingRect(const QRect rect)
{
    _heightForWidthRatio = (qreal)rect.size().height() / (qreal)rect.size().width();
    if (_image) delete _image;
    _image = new QImage(rect.size(), QImage::Format_RGB32);
    _drawingRect = rect;
}

void MinoProgram::addAnimation(MinoAnimation *animation)
{
    // Add animation to program's list
    _minoAnimations.append(animation);

    // Will remove animation from list when destroyed
    connect(animation, SIGNAL(destroyed(QObject*)), this, SLOT(destroyAnimation(QObject*)));

    // Set position to origin
    _itemGroup.setPos(0,0);

    // Add to program QGraphicsItemGroup to ease group manipulation (ie. change position, brightness, etc.)
    _itemGroup.addToGroup(animation->itemGroup());

    // Re-parent animation to our itemGroup
    animation->itemGroup()->setParentItem(&_itemGroup);
}

MinoAnimation* MinoProgram::addAnimation(const QString animationClassName)
{
    MinoAnimation *animation = MinoAnimationFactory::createObject(animationClassName.toAscii(), Minotor::minotor());
    if(animation)
    {
        addAnimation(animation);
    }
    return animation;
}

void MinoProgram::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    // Set position to origin
    _itemGroup.setPos(0,0);

    // Animate whole content (ie. this includes objects creation/desctruction moves)
    foreach(MinoAnimation *minoAnimation, _minoAnimations)
        minoAnimation->animate(gppqn, ppqn, qn);

    // Reset position to the affected one_itemGroup
    _itemGroup.setPos(_drawingRect.topLeft());

    /*
    qDebug() << "MinoChannel->animate"
             << "_drawingRect" << _drawingRect;
    */

    // Set background
    _image->fill(Qt::black);

    // Render the scene at previously saved view rect
    QPainter painter(_image);
    _scene->render(&painter, QRectF(_image->rect()), _drawingRect, Qt::IgnoreAspectRatio);

    // Let's connected object to know the program's animation is done
    emit animated();
}

void MinoProgram::destroyAnimation(QObject *animation)
{
    _minoAnimations.removeAt(_minoAnimations.indexOf(static_cast<MinoAnimation*>(animation)));
}
