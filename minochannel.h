#ifndef MINOCHANNEL_H
#define MINOCHANNEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsView>

#include "minoanimation.h"
#include "minomatrixedscenerenderer.h"

class MinoChannel : public QObject
{
    Q_OBJECT
public:
    explicit MinoChannel(const QSize size, QObject *parent);
    ~MinoChannel();
    QGraphicsScene *scene() { return &_scene; }
    MinoMatrixedSceneRenderer *renderer() { return _renderer; }

    //QGraphicsView *view() { return &_view; }
    MinoAnimationList animations() { return _minoAnimations; }
private:
    QSize _size;
    QGraphicsScene _scene;
    MinoMatrixedSceneRenderer *_renderer;
    QGraphicsView _view;
    MinoAnimationList _minoAnimations;
    QGraphicsItemGroup _itemGroup;

signals:
    void customContextMenuRequested(const QPoint &pos);
    void animated();
public slots:
    void animate(const unsigned int ppqn);
};

#endif // MINOCHANNEL_H
