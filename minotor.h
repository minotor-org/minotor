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
#include "minoclocksource.h"

class MinoAnimation;
typedef QList<MinoAnimation*> MinoAnimationList;

class MinoChannel;

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(QObject *parent);

    // Channel accessors
    MinoChannel* channel1() { return _channel1; }
    MinoChannel* channel2() { return _channel2; }
    MinoMaster* master() { return _master; }

    // LedMatrix
    LedMatrix* ledMatrix() { return _ledMatrix; }

    // MIDI
    // TODO Support more than one interface
    Midi *midi() { return _midiInterfaces.at(0); }

    // MIDI mapping
    MidiMapping *midiMapping() { return &_midiMapping; }

    // Clock source
    MinoClockSource *clockSource() { return _clockSource; }
signals:
    void colorControlChanged(int value);
    void controlChanged(int midiInterfaceId, quint8 channel, quint8 control, quint8 value);

public slots:
    // Clock handler
    void dispatchClock(const int ppqn, const int qn);

    // Midi messages handlers
    void handleMidiInterfaceControlChange(quint8 channel, quint8 control, quint8 value);

private:
    // External connections
    LedMatrix *_ledMatrix;

    // Midi interfaces
    MidiInterfaceList _midiInterfaces;

    // Midi mapping
    MidiMapping _midiMapping;

    MinoClockSource *_clockSource;

    MinoMaster *_master;
    MinoChannel *_channel1;
    MinoChannel *_channel2;
};

#endif // MINOTOR_H
