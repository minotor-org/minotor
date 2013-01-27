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

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Waveform", "YYY", QPixmap(), "MinoAnimationWaveform");
    }
    const MinoAnimationDescription description() const { return MinoAnimationWaveform::getDescription(); }

signals:
    
public slots:

private:
    MinoAnimationProperty _color;
};

#endif // MINOANIMATIONWAVEFORM_H
