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

class MinoAnimation;
typedef QList<MinoAnimation*> MinoAnimationList;

class MinoChannel;

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(Midi *midi, QObject *parent = 0);

    // Channel accessors
    MinoChannel* channel1() { return _channel1; }
    MinoChannel* channel2() { return _channel2; }
    MinoMaster* master() { return _master; }

    // LedMatrix
    void setLedMatrix(LedMatrix *ledMatrix);
    LedMatrix* ledMatrix() { return _ledMatrix; }
    // MIDI mapping
    MidiMapping *midiMapping() { return &_midiMapping; }

signals:
    void colorControlChanged(int value);
    void controlChanged(int midiInterfaceId, quint8 channel, quint8 control, quint8 value);

public slots:
    // Midi messages handlers
    void handleClock();
    void handleStop();
    void handleStart();
    void handleContinue();
    void handleMidiInterfaceControlChange(quint8 channel, quint8 control, quint8 value);

    void animate(const int ppqn);
private:
    // External connections
    LedMatrix *_ledMatrix;

    // Midi interfaces
    QMidiInterfaceList _midiInterfaces;
    // Midi mapping
    MidiMapping _midiMapping;

    // Sequence watching
    unsigned int _ppqnId;    //pulse per quarter note's ID (ie. 0 -> ... -> 23 -> 0) Note: a "start" midi message resets this counter
    bool _isSequenceRunning; //sets when midi device is in "running mode" (ie. after "start" or "continue" midi message)

    MinoMaster *_master;
    MinoChannel *_channel1;
    MinoChannel *_channel2;
};

#endif // MINOTOR_H
