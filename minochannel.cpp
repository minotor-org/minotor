#include "minochannel.h"

#include "minotor.h"

#include <QBrush>
#include <QDebug>

MinoChannel::MinoChannel(Minotor *minotor, const QRect drawingRect) :
    QObject(minotor),
    _minotor(minotor),
    _image(NULL)
{
    _scene = minotor->scene();

    setDrawingRect(drawingRect);

    _scene->addItem(&_itemGroup);
}

MinoChannel::~MinoChannel()
{
    foreach (MinoAnimation *animation, _minoAnimations)
    {
        delete(animation);
    }
    delete _image;
}

void MinoChannel::setDrawingRect(const QRect rect)
{
    _heightForWidthRatio = (qreal)rect.size().height() / (qreal)rect.size().width();
    if (_image) delete _image;
    _image = new QImage(rect.size(), QImage::Format_RGB32);
    _drawingRect = rect;
}

void MinoChannel::addAnimation(MinoAnimation *animation)
{
    _minoAnimations.append(animation);
    _itemGroup.addToGroup(animation->itemGroup());
}

MinoAnimation* MinoChannel::addAnimation(const QString animationClassName)
{
    MinoAnimation *animation = MinoAnimationFactory::createObject(animationClassName.toAscii(), _minotor);
    if(animation)
    {
        addAnimation(animation);
    }
    return animation;
}

void MinoChannel::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    // Set position to origin
    _itemGroup.setPos(0,0);

    // Animate whole content (ie. this includes objects creation/desctruction moves)
    foreach(MinoAnimation *minoAnimation, _minoAnimations)
        minoAnimation->animate(gppqn, ppqn, qn);

    // Reset position to the affected one
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

    // Let's connected object to know the channel's animation is done
    emit animated();
}
