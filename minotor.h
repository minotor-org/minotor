#ifndef MINOTOR_H
#define MINOTOR_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>

#include "ledmatrix.h"
#include "midi.h"
#include "midimapping.h"

#include "minomaster.h"
#include "minocue.h"
#include "minoclocksource.h"

#include "minoanimationfactory.h"

class MinoAnimation;
typedef QList<MinoAnimation*> MinoAnimationList;

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(QObject *parent);

    // Scene accessor
    QGraphicsScene *scene() { return &_scene; }

    // Channel accessors
    MinoMaster *master() { return _master; }
    MinoCue *cue() { return _cue; }

    // LedMatrix
    LedMatrix *ledMatrix() { return _ledMatrix; }

    // MIDI
    // TODO Support more than one interface
    Midi *midi() { return _midiInterfaces.at(0); }

    // MIDI mapping
    MidiMapping *midiMapping() { return &_midiMapping; }

    // Clock source
    MinoClockSource *clockSource() { return _clockSource; }

    // Animation factory
    MinoAnimationFactory *animationFactory() { return &_animationFactory; }

signals:
    void controlChanged(int midiInterfaceId, quint8 channel, quint8 control, quint8 value);

public slots:
    // Clock handler
    void dispatchClock(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    // Midi messages handlers
    void handleMidiInterfaceControlChange(quint8 channel, quint8 control, quint8 value);

private:
    // Scene
    QGraphicsScene _scene;

    // Channels
    MinoMaster *_master;
    MinoCue *_cue;

    // External connections
    LedMatrix *_ledMatrix;

    // Midi interfaces
    MidiInterfaceList _midiInterfaces;

    // Midi mapping
    MidiMapping _midiMapping;

    // Clock source (internal generator and Midi)
    MinoClockSource *_clockSource;

    // Animation factory
    MinoAnimationFactory _animationFactory;
};

#endif // MINOTOR_H
