#ifndef MINOANIMATIONWAVEFORM_H
#define MINOANIMATIONWAVEFORM_H

#include "minoanimation.h"
#include "minoanimationproperty.h"

class MinoAnimationWaveform : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationWaveform(MinoChannel *channel);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);
    qreal ratioToBeatFactor(qreal value);

signals:
    
public slots:

private:
    MinoAnimationProperty _beatFactor;
    MinoAnimationProperty _color;
};

#endif // MINOANIMATIONWAVEFORM_H
