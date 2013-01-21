#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"

class MinoAnimationExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationExpandingObjects(QGraphicsScene* scene, QObject *parent = 0);
    void animate(const unsigned int ppqn);
signals:
    
public slots:
    
private:
    MinoAnimationProperty _color;
};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H
