#ifndef MINOCHANNEL_H
#define MINOCHANNEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QGraphicsView>

#include "minoanimation.h"

class MinoChannel : public QObject
{
    Q_OBJECT
public:
    explicit MinoChannel(QObject *parent = 0);
    ~MinoChannel();
    QGraphicsScene *scene() { return &_scene; }
    QGraphicsView *view() { return &_view; }
    MinoAnimationList animations() { return _minoAnimations; }
private:
    QGraphicsScene _scene;
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
