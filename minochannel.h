#ifndef MINOCHANNEL_H
#define MINOCHANNEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QRect>

#include "minoanimation.h"
#include "minomatrixedscenerenderer.h"

class Minotor;

class MinoChannel : public QObject
{
    Q_OBJECT
public:
    explicit MinoChannel(Minotor *minotor);
    ~MinoChannel();

    virtual QString name() = 0; // Force MinoChannel to become a pure virtual class

    MinoMatrixedSceneRenderer *renderer() { return _renderer; }

    void setDrawingRect(const QRect rect);

    MinoAnimationList animations() { return _minoAnimations; }

    QGraphicsScene *scene() { return _scene; }
    QRect boundingRect() { return QRect(0,0,_drawingRect.width(),_drawingRect.height()); }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }

private:
    QRect _drawingRect;
    QGraphicsScene *_scene;
    MinoMatrixedSceneRenderer *_renderer;

protected:
    MinoAnimationList _minoAnimations;
    QGraphicsItemGroup _itemGroup;

signals:
    void customContextMenuRequested(const QPoint &pos);
    void animated();
public slots:
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);
};

#endif // MINOCHANNEL_H
