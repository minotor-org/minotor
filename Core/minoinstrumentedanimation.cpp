#include "minoinstrumentedanimation.h"

MinoInstrumentedAnimation::MinoInstrumentedAnimation(QObject *parent) :
    MinoAnimation(parent)
{
}

QColor MinoInstrumentedAnimation::noteToColor(const quint8 note)
{
    QColor color;
    const unsigned int subnote = note % 12;
    const qreal hue = (qreal)subnote / 12;
    const qreal lightness = (qreal)note / 127;
    color.setHslF(hue, 1.0, lightness);
    return color;
}
