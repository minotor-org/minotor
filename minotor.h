#ifndef MINOTOR_H
#define MINOTOR_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>

#include "ledmatrix.h"
#include "midi.h"

class MinoAnimation;

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(Midi *midi, QObject *parent = 0);
    QGraphicsScene* scene();

    void setLedMatrix(LedMatrix *ledMatrix);
signals:
    void colorControlChanged(int value);

public slots:
    // Midi messages handlers
    void handleClock();
    void handleStop();
    void handleStart();
    void handleContinue();
    void handleControlChange(quint8 control, quint8 value);
private:
    // External connections
    LedMatrix *_ledMatrix;

    // Animations
    QGraphicsScene _scene;
    MinoAnimation *_minoAnimation1;
    MinoAnimation *_minoAnimation2;

    // Sequence watching
    unsigned int _ppqnId;    //pulse per quarter note's ID (ie. 0 -> ... -> 23 -> 0) Note: a "start" midi message resets this counter
    bool _isSequenceRunning; //sets when midi device is in "running mode" (ie. after "start" or "continue" midi message)

    QGraphicsItemGroup _mainItemGroup;
};

#endif // MINOTOR_H
