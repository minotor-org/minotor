#include "minoinstrumentedanimation.h"

MinoInstrumentedAnimation::MinoInstrumentedAnimation(QObject *parent) :
    MinoAnimation(parent),
    _alive(false)
{
    _midiChannel = new MinoPropertyMidiChannel(this);
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

void MinoInstrumentedAnimation::setAlive(const bool on)
{
    // FIXME: setAlive() is called when MinoAnimation::setEnabled() is used, so we can't handle setAlive(false) here
    if(on)
    {
        MinoAnimation::setAlive(true);
        _alive = on;
    }
    else
    {
        // Nothing to do: we let animate() detected that if animation is alive...
    }
}

void MinoInstrumentedAnimation::handleNoteChange(int interface, quint8 channel, quint8 note, bool on, quint8 value)
{
    (void)interface;
    if((_midiChannel->channel()) && (channel==_midiChannel->channel()-1))
    {
        _handleNoteChange(note, on, value);
    }
}
