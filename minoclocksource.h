#ifndef MINOCLOCKSOURCE_H
#define MINOCLOCKSOURCE_H

#include <QObject>

#include "midi.h"

#include <QTimer>
#include <QTime>

class MinoClockSource : public QObject
{
    Q_OBJECT
public:
    explicit MinoClockSource(QObject *parent = 0);
    void setMidiClockInterface(Midi *midi);
    qreal bpm() const { return (60000.0 / _bpmPeriodMs); }
signals:
    // Signal emitting a pre-computed pulse-per-quarter-note and quarter-note id (less code in receiver-classes, ie. MinoAnimations)
    void clock(const int ppqn, const int qn);

    // Signal emitting the BPM value changed (useful for UI feedback)
    void bpmChanged(const double bpm);

public slots:
    // Internal generator controls
    void uiTapOn();
    void uiStart();
    void uiStop();
    void uiSync();

private:
    // Current Pulse-Per-Quarter-Note and Quater-Note Id (in range of [0-24[ and [0-16[ respectively
    int _ppqn;
    int _qn;

    // Current tempo, please note that we don't store in BPM unit to prevent from precision lose
    qreal _bpmPeriodMs; // Unit is milliseconds between two beats

    // BPM tap count elapsed time between two tap
    QTime _bpmTap;
    int _bpmValues[10]; // Array used for average
    int _bpmValuesCount;
    int _bpmValuesIndex;
    qreal _bpmAverageMs;

    // Internal timer is used to produce an internal clock
    QTimer _internalTimer;

    // MIDI
    bool _isMidiSequencerRunning; //sets when midi device is in "running mode" (ie. after "start" or "continue" midi message)

private slots:
    // MIDI
    void midiClock();
    void midiStop();
    void midiStart();
    void midiContinue();

    // Internal generator
    void internalTimerTimeout();
};

#endif // MINOCLOCKSOURCE_H
