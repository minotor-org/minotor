#include "minoinstrumentedanimation.h"
#include "minotor.h"
#include "minoanimationgroup.h"

#include <QDebug>

MinoInstrumentedAnimation::MinoInstrumentedAnimation(QObject *parent) :
    MinoAnimation(parent),
    _alive(false),
    _itemCreationRequested(0)
{
    _midiChannel = new MinoPropertyMidiChannel(this);
}

QColor MinoInstrumentedAnimation::noteToColor(const quint8 note, const QColor& colorBase)
{
    QColor color;

    Q_ASSERT(colorBase.isValid());

    const unsigned int subnote = note % 12;

    qreal hue = (qreal)subnote / 12.0;
    hue += colorBase.hueF();
    if(hue>1.0) hue = hue - 1.0;

    // HACK to support lightness == 1.0
    hue = qMax(0.0, hue);

    const qreal lightnessBase = colorBase.lightnessF();

    qreal lightness = (qreal)note / 127.0;
    lightness = lightness - 0.5;

    qreal lightnessRange = 0.5 - lightnessBase;
    lightnessRange = (0.5 - qAbs(lightnessRange)) * 2.0;

    lightness = (lightness*lightnessRange) + lightnessBase;

    color.setHslF(hue, colorBase.saturationF(), lightness);
    Q_ASSERT(color.isValid());
    return color;
}

unsigned int MinoInstrumentedAnimation::noteToPosX(const quint8 note, const unsigned int reservedSize)
{
    unsigned int posX = 0;
    const unsigned int subnote = note % 12;
    const qreal step = ((qreal)_boundingRect.width()-(qreal)reservedSize) / 12;
    posX = step * qreal(subnote);
    return posX;
}

unsigned int MinoInstrumentedAnimation::noteToPosY(const quint8 note, const unsigned int reservedSize)
{
    unsigned int posY = 0;
    const unsigned int subnote = note % 12;
    const qreal step = ((qreal)_boundingRect.height()-(qreal)reservedSize) / 12;
    posY = step * qreal(subnote);
    return posY;
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

void MinoInstrumentedAnimation::createItem()
{
    _itemCreationRequested++;

    setAlive(true);
    MinoAnimationGroup* mag = qobject_cast<MinoAnimationGroup*>(parent());
    Q_ASSERT(mag);
    mag->setAlive();
}

void MinoInstrumentedAnimation::processItemCreation(const uint uppqn)
{
    for (int i = 0; i<_itemCreationRequested; i++)
    {
        _createItem(uppqn);
    }
    _itemCreationRequested = 0;
}

void MinoInstrumentedAnimation::handleNoteChange(int interface, quint8 channel, quint8 note, bool on, quint8 value)
{
    (void)interface;
    if((_midiChannel->channel()) && (channel==_midiChannel->channel()-1))
    {
        _noteEvents.append(MinoInstrumentNoteEvent(note, on, value));

        setAlive(true);
        MinoAnimationGroup* mag = qobject_cast<MinoAnimationGroup*>(parent());
        Q_ASSERT(mag);
        mag->setAlive();
    }
}

void MinoInstrumentedAnimation::processNotesEvents(const uint uppqn)
{
    foreach(MinoInstrumentNoteEvent ne, _noteEvents)
    {
        if(ne.on())
        {
            _startNote(uppqn, ne.note(), ne.value());
            _pendingNotes.append(ne.note());
        } else {
            _stopNote(uppqn, ne.note());
            _pendingNotes.removeAt(_pendingNotes.indexOf(ne.note()));
        }
    }
    _noteEvents.clear();
    foreach(int note, _pendingNotes)
    {
        _processPendingNote(uppqn, note);
    }
}
