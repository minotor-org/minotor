#include "minochannel.h"

#include "minoanimationrandompixels.h"
#include "minoanimationexpandingobjects.h"
#include "minoanimationdebug.h"
#include "minoanimationwaveform.h"
#include "minoanimationbarsfromsides.h"

#include <QBrush>

MinoChannel::MinoChannel(const QSize size, QObject *parent) :
    QObject(parent),
    _size(size)
{
    // XXX How to resize channel ?
    _scene.setSceneRect(QRectF(0, 0, size.width(), size.height()));

    //_minoAnimations.append(new MinoAnimationDebug(&_scene, this));
    //_minoAnimations.append(new MinoAnimationRandomPixels(&_scene, this));
    //_minoAnimations.append(new MinoAnimationExpandingObjects(&_scene, this));
    //_minoAnimations.append(new MinoAnimationWaveform(&_scene, this));
    //_minoAnimations.append(new MinoAnimationWaveform(&_scene, this));
    _minoAnimations.append(new MinoAnimationBarsFromSides(&_scene, this));

    foreach(MinoAnimation *minoAnimation, _minoAnimations)
    {
        _itemGroup.addToGroup(minoAnimation->itemGroup());
    }
    _scene.addItem(&_itemGroup);

    _view.setScene(&_scene);

    _view.setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    _view.setTransformationAnchor(QGraphicsView::NoAnchor);
    _view.setResizeAnchor(QGraphicsView::NoAnchor);
    _view.setInteractive(false);

    _view.viewport()->setFixedSize(size);
    _view.setFixedSize(size);
    _view.setSceneRect(QRectF(0, 0, size.width(), size.height()));
    _view.fitInView(_view.sceneRect());

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

