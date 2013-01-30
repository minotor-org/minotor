#ifndef MINOCHANNEL_H
#define MINOCHANNEL_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QRect>

#include "minoanimation.h"

class Minotor;

class MinoChannel : public QObject
{
    Q_OBJECT
public:
    explicit MinoChannel(Minotor *minotor, const QRect drawingRect);
    ~MinoChannel();

    virtual QString name() = 0; // Force MinoChannel to become a pure virtual class

    // After MinoChannel creation, user should be able to set (and reset) drawing rect
    void setDrawingRect(const QRect rect);

    void addAnimation(MinoAnimation *animation);
    MinoAnimation* addAnimation(const QString animationClassName);

    // Accessors
    QGraphicsScene *scene() { return _scene; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
    MinoAnimationList animations() { return _minoAnimations; }
    const QImage *rendering() { return _image; }

    QRect boundingRect() { return QRect(0,0,_drawingRect.width(),_drawingRect.height()); }

    // Function is compute height with a given width (very useful for UI)
    int heightForWidth( int width ) const { return (qreal)width * _heightForWidthRatio; }

private:
    // Minotor
    Minotor *_minotor;

    // Scene
    QGraphicsScene *_scene;

    // Drawing rect (in scene coordinates)
    QRect _drawingRect;

    // QImage to store rendering
    QImage *_image;

    // Image ratio
    qreal _heightForWidthRatio;

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
