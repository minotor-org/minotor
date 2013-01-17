#ifndef MINOANIMATIONWAVEFORM_H
#define MINOANIMATIONWAVEFORM_H

#include "minoanimation.h"
#include "minoanimationproperty.h"

class MinoAnimationWaveform : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationWaveform(QGraphicsScene *scene, QObject *parent = 0);
    void animate(const unsigned int ppqn);
    qreal ratioToBeatFactor(qreal value);
signals:
    
public slots:

private:
    MinoAnimationProperty _beatFactor;
    MinoAnimationProperty _color;
};

#endif // MINOANIMATIONWAVEFORM_H
