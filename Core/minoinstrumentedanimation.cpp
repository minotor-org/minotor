#include "minoinstrumentedanimation.h"

#include "minoanimationgroup.h"

MinoInstrumentedAnimation::MinoInstrumentedAnimation(QObject *parent) :
    MinoAnimation(parent),
    _alive(false),
    _itemCreationRequested(0)
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
    foreach(int note, _pendingNotes)
    {
        _processPendingNote(uppqn, note);
    }
}
