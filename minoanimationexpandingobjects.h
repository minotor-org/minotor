#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"

class MinoAnimationExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationExpandingObjects(Minotor *parent);
    void animate(const unsigned int ppqn);
signals:
    
public slots:
    
};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H
