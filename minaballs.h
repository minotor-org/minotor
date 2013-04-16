#ifndef MINABALLS_H
#define MINABALLS_H

#include "minoanimation.h"

#include <QGraphicsRectItem>
#include <QLineF>
#include <QList>

class MinoAnimatedBall : public MinoAnimatedItem
{
public:
    explicit MinoAnimatedBall(QLineF path, QLineF destination, const unsigned int startUppqn,const unsigned int duration,QGraphicsItem *graphicsItem):
        MinoAnimatedItem(startUppqn, duration, graphicsItem),
        _path(path),
        _destination(destination)
    {
    }

    QLineF _path;
    QLineF _destination;
} ;

class MinaBalls : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaBalls(QObject *object);

    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("balls", "Balls", QPixmap(":/images/balls.png"), MinaBalls::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItemGroup* graphicItem() { return &_itemGroup; }

    //void setDuration(const unsigned int duration) { _beatDuration.setCurrentItem(QString::number(duration)); }
private:
    QGraphicsItemGroup _itemGroup;
    QList<MinoAnimatedBall> _animatedItems;
    QList<QLineF> _borders;

};

#endif // MINABALLS_H
