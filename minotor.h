#ifndef MINOTOR_H
#define MINOTOR_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>

#include "ledmatrix.h"

#include "midi.h"
#include "midimapping.h"

class MinoAnimation;
typedef QList<MinoAnimation*> MinoAnimationList;

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(Midi *midi, QObject *parent = 0);
    QGraphicsScene* scene();

    void setLedMatrix(LedMatrix *ledMatrix);

    MidiMapping *midiMapping() { return &_midiMapping; }
    MinoAnimationList animations() { return _minoAnimations; }
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
private:
    // External connections
    LedMatrix *_ledMatrix;

    // Animations
    QGraphicsScene _scene;
    MinoAnimationList _minoAnimations;

    // Midi interfaces
    QMidiInterfaceList _midiInterfaces;
    // Midi mapping
    MidiMapping _midiMapping;

    // Sequence watching
    unsigned int _ppqnId;    //pulse per quarter note's ID (ie. 0 -> ... -> 23 -> 0) Note: a "start" midi message resets this counter
    bool _isSequenceRunning; //sets when midi device is in "running mode" (ie. after "start" or "continue" midi message)

    QGraphicsItemGroup _mainItemGroup;
};

#endif // MINOTOR_H
