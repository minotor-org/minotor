#ifndef MINOANIMATIONRANDOMPIXELS_H
#define MINOANIMATIONRANDOMPIXELS_H

#include "minoanimation.h"

class MinoAnimationRandomPixels : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationRandomPixels(Minotor *parent);
    void animate(const unsigned int ppqn);
signals:
    
public slots:
    void colorControlChanged(int value);
private:
    int _color;
};

#endif // MINOANIMATIONRANDOMPIXELS_H
