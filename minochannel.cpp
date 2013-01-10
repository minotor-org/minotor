#include "minochannel.h"

#include "minoanimationrandompixels.h"
#include "minoanimationexpandingobjects.h"
#include "minoanimationdebug.h"

#include <QBrush>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>

MinoChannel::MinoChannel(QObject *parent) :
    QObject(parent)
{
    // TODO Remove hardcoded values
    _scene.setSceneRect(QRectF(0, 0, 240, 160));

    _minoAnimations.append(new MinoAnimationDebug(&_scene, this));
    //_minoAnimations.append(new MinoAnimationRandomPixels(&_scene, this));
    _minoAnimations.append(new MinoAnimationExpandingObjects(&_scene, this));

    foreach(MinoAnimation *minoAnimation, _minoAnimations)
    {
        _itemGroup.addToGroup(minoAnimation->itemGroup());
    }
    _scene.addItem(&_itemGroup);

    _view.setScene(&_scene);
    _view.setSceneRect(QRectF(0, 0, 240, 160));
    _view.setFixedSize(240,160);
    _view.resize(240,160);
    _view.viewport()->setFixedSize(240,160);
    _view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _view.setFrameShape(QFrame::NoFrame);
    _view.setStyleSheet("background:transparent;");
    _view.setAttribute(Qt::WA_TranslucentBackground);
    _view.setWindowFlags(Qt::FramelessWindowHint);
}

MinoChannel::~MinoChannel()
{
    foreach (MinoAnimation *animation, _minoAnimations)
    {
        delete(animation);
    }
}

void MinoChannel::animate(const unsigned int ppqn)
{
    foreach(MinoAnimation *minoAnimation, _minoAnimations)
        minoAnimation->animate(ppqn);
    emit animated();
}

