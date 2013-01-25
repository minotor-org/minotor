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
    _renderer = new MinoMatrixedSceneRenderer(&_scene);
    connect(this, SIGNAL(animated()), _renderer, SLOT(render()));

    // TODO Remove hardcoded values!
    _renderer->setMatrixSize(QSize(24, 16));
    _renderer->setViewRect(QRect(0, 0, 24, 16));

    _minoAnimations.append(new MinoAnimationDebug(&_scene, this));
    //_minoAnimations.append(new MinoAnimationRandomPixels(&_scene, this));
    //_minoAnimations.append(new MinoAnimationExpandingObjects(&_scene, this));
    //_minoAnimations.append(new MinoAnimationWaveform(&_scene, this));
    //_minoAnimations.append(new MinoAnimationBarsFromSides(&_scene, this));

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

void MinoChannel::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    foreach(MinoAnimation *minoAnimation, _minoAnimations)
        minoAnimation->animate(gppqn, ppqn, qn);
    emit animated();
}

