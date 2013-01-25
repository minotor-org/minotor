#ifndef MINOANIMATIONPLASMA_H
#define MINOANIMATIONPLASMA_H

#include "minoanimation.h"

class MinoAnimationPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationPlasma(QGraphicsScene* scene, QObject *parent = 0);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    qreal ratioToBeatFactor(qreal value);
private:
    MinoAnimationProperty _beatFactor;
    MinoAnimationProperty _color;
signals:

public slots:

};

#endif // MINOANIMATIONPLASMA_H
