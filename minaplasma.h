#ifndef MINOANIMATIONPLASMA_H
#define MINOANIMATIONPLASMA_H

#include "minoanimation.h"

class MinaPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaPlasma(Minotor *minotor);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Plasma", "YYY", QPixmap(), "MinoAnimationPlasma");
    }
    const MinoAnimationDescription description() const { return MinaPlasma::getDescription(); }

private:
    MinoAnimationProperty _color;

signals:

public slots:

};

#endif // MINOANIMATIONPLASMA_H
