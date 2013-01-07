#ifndef MINOANIMATIONRANDOMPIXELS_H
#define MINOANIMATIONRANDOMPIXELS_H

#include "minoanimation.h"
#include "minoanimationproperty.h"

class MinoAnimationRandomPixels : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationRandomPixels(Minotor *parent);
    void animate(const unsigned int ppqn);
signals:
    
public slots:

private:
    MinoAnimationProperty _color;
};

#endif // MINOANIMATIONRANDOMPIXELS_H
