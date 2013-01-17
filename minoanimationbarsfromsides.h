#ifndef MINOANIMATIONBARSFROMSIDES_H
#define MINOANIMATIONBARSFROMSIDES_H

#include "minoanimation.h"

class MinoAnimationBarsFromSides : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationBarsFromSides(QGraphicsScene* scene, QObject *parent = 0);
    void animate(const unsigned int ppqn);

    qreal ratioToBeatFactor(qreal value);
private:
    MinoAnimationProperty _beatFactor;
    MinoAnimationProperty _color;
signals:

public slots:
    
};

#endif // MINOANIMATIONBARSFROMSIDES_H
