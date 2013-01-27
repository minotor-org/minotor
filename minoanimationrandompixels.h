#ifndef MINOANIMATIONRANDOMPIXELS_H
#define MINOANIMATIONRANDOMPIXELS_H

#include "minoanimation.h"
#include "minoanimationproperty.h"

class MinoAnimationRandomPixels : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationRandomPixels(MinoChannel *channel);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Random pixels", "ZZZ", QPixmap(), "MinoAnimationRandomPixels");
    }
    const MinoAnimationDescription description() const { return MinoAnimationRandomPixels::getDescription(); }

signals:
    
public slots:

private:
    MinoAnimationProperty _color;
    MinoAnimationProperty _density;
};

#endif // MINOANIMATIONRANDOMPIXELS_H
