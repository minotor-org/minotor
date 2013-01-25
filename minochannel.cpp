#include "minochannel.h"

#include "minotor.h"

#include "minoanimationrandompixels.h"
#include "minoanimationexpandingobjects.h"
#include "minoanimationdebug.h"
#include "minoanimationwaveform.h"
#include "minoanimationbarsfromsides.h"

#include <QBrush>
#include <QDebug>

MinoChannel::MinoChannel(Minotor *minotor) :
    QObject(minotor)
{
    _scene = minotor->scene();

    _renderer = new MinoMatrixedSceneRenderer(_scene);
    connect(this, SIGNAL(animated()), _renderer, SLOT(render()));

    // This line is needed due to hard-coded animation entries...
    // FIXME Removes hard-coded animations, then this following hack-line
    setDrawingRect(QRect(0,0,24,16));

    _minoAnimations.append(new MinoAnimationDebug(this));
    //_minoAnimations.append(new MinoAnimationRandomPixels(this));
    //_minoAnimations.append(new MinoAnimationExpandingObjects(this));
    //_minoAnimations.append(new MinoAnimationWaveform(this));
    //_minoAnimations.append(new MinoAnimationBarsFromSides(this));

    foreach(MinoAnimation *minoAnimation, _minoAnimations)
    {
        _itemGroup.addToGroup(minoAnimation->itemGroup());
    }
    _scene->addItem(&_itemGroup);
}

MinoChannel::~MinoChannel()
{
    foreach (MinoAnimation *animation, _minoAnimations)
    {
        delete(animation);
    }
}

void MinoChannel::setDrawingRect(const QRect rect)
{
    _renderer->setMatrixSize(rect.size());
    _renderer->setViewRect(rect);
    _drawingRect = rect;
}

void MinoChannel::animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    _itemGroup.setPos(0,0);
    foreach(MinoAnimation *minoAnimation, _minoAnimations)
        minoAnimation->animate(gppqn, ppqn, qn);

    _itemGroup.setPos(_drawingRect.topLeft());
    qDebug() << "MinoChannel->animate"
             << "_drawingRect" << _drawingRect;
    emit animated();
}

