#ifndef MINOPROGRAM_H
#define MINOPROGRAM_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QRect>

#include "minoanimation.h"

class Minotor;

class MinoProgram : public QObject
{
    Q_OBJECT

    // Minotor can access to protected/private functions
    friend class Minotor;

public:
    explicit MinoProgram(Minotor *minotor);
    ~MinoProgram();

    virtual QString name() = 0; // Force MinoChannel to become a pure virtual class

    // Animation manipulation
    void addAnimation(MinoAnimation *animation);
    MinoAnimation* addAnimation(const QString animationClassName);
    void registerAnimationEnableChange(MinoAnimation *animation, const bool on);

    // Accessors
    QGraphicsScene *scene() { return _scene; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
    MinoAnimationList animations() { return _minoAnimations; }
    const QImage *rendering() { return _image; }
    int id() { return _id; }
    // Selection
    bool isSelected() { return true; }

    // Function is compute height with a given width (very useful for UI)
    int heightForWidth( int width ) const { return (qreal)width * _heightForWidthRatio; }

    Minotor *minotor();

protected:
    // At end of object creation, Minotor will set ID and drawing rect
    void setId(const int id) { _id = id; }
    void setRect(const QRect rect);
    void setDrawingPos(const QPointF pos);

private:
    // ID
    int _id;

    // Scene
    QGraphicsScene *_scene;

    // Drawing rect (in scene coordinates)
    QRect _rect;
    QPointF _drawingPos;

    // QImage to store rendering
    QImage *_image;

    // Image ratio
    qreal _heightForWidthRatio;

protected:
    MinoAnimationList _minoAnimations;
    MinoAnimationList _minoAnimationsToEnable;
    MinoAnimationList _minoAnimationsToDisable;

    QGraphicsItemGroup _itemGroup;

signals:
    void customContextMenuRequested(const QPoint &pos);
    void animated();

public slots:
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);
    void destroyAnimation(QObject *animation);
};

#endif // MINOPROGRAM_H
