#ifndef MINOANIMATIONBARSFROMSIDES_H
#define MINOANIMATIONBARSFROMSIDES_H

#include "minoanimation.h"

class MinoAnimationBarsFromSides : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationBarsFromSides(MinoChannel *channel);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Bars from sides", "XXX", QPixmap(), "MinoAnimationBarsFromSides");
    }
    const MinoAnimationDescription description() const { return MinoAnimationBarsFromSides::getDescription(); }

private:
    MinoAnimationProperty _color;
signals:

public slots:
    
};

#endif // MINOANIMATIONBARSFROMSIDES_H
