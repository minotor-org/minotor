#ifndef MINOANIMATIONDEBUG_H
#define MINOANIMATIONDEBUG_H

#include "minoanimation.h"

class MinoAnimationDebug : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationDebug(QGraphicsScene* scene, QObject *parent = 0);
    void animate(const unsigned int ppqn);
private:
    MinoAnimationProperty _r;
    MinoAnimationProperty _g;
    MinoAnimationProperty _b;
signals:
    
public slots:
    
};

#endif // MINOANIMATIONDEBUG_H
