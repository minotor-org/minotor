#ifndef MINOANIMATIONBARSFROMSIDES_H
#define MINOANIMATIONBARSFROMSIDES_H

#include "minoanimation.h"

class MinoAnimationBarsFromSides : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationBarsFromSides(MinoChannel *channel);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    qreal ratioToBeatFactor(qreal value);
private:
    MinoAnimationProperty _beatFactor;
    MinoAnimationProperty _color;
signals:

public slots:
    
};

#endif // MINOANIMATIONBARSFROMSIDES_H
