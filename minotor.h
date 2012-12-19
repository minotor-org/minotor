#ifndef MINOTOR_H
#define MINOTOR_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>

#include "ledmatrix.h"
#include "midi.h"

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(Midi *midi, QObject *parent = 0);
    QGraphicsScene* scene();

    void setLedMatrix(LedMatrix *ledMatrix);
signals:
    
public slots:
    // Midi messages handlers
    void handleClock();
    void handleStop();
    void handleStart();
    void handleContinue();
private:
    // External connections
    LedMatrix *_ledMatrix;

    // Animations
    QGraphicsScene _scene;
    QPropertyAnimation _animation;

    // Sequence watching
    unsigned int _ppqnId;    //pulse per quarter note's ID (ie. 0 -> ... -> 23 -> 0) Note: a "start" midi message resets this counter
    bool _isSequenceRunning; //sets when midi device is in "running mode" (ie. after "start" or "continue" midi message)

    QGraphicsItemGroup _mainItemGroup;
};

#endif // MINOTOR_H
