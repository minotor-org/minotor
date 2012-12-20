#ifndef MINOANIMATION_H
#define MINOANIMATION_H

#include <QObject>

#include "minotor.h"

class MinoAnimation : public QObject
{
    Q_OBJECT
public:
    explicit MinoAnimation(Minotor *parent);
    virtual void animate(const unsigned int ppqn) = 0;
    QGraphicsItemGroup* itemGroup();

protected:
    QPropertyAnimation _animatedProperty;
    QGraphicsItemGroup _itemGroup;
signals:
    
public slots:
    
};

#endif // MINOANIMATION_H
