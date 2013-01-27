#ifndef MINOANIMATIONPLASMA_H
#define MINOANIMATIONPLASMA_H

#include "minoanimation.h"

class MinoAnimationPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationPlasma(MinoChannel *channel);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Plasma", "YYY", QPixmap(), "MinoAnimationPlasma");
    }
    const MinoAnimationDescription description() const { return MinoAnimationPlasma::getDescription(); }

private:
    MinoAnimationProperty _color;

signals:

public slots:

};

#endif // MINOANIMATIONPLASMA_H
