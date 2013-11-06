/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    explicit MinoClockSource(QObject *parent);
    void setMidiClockSource(Midi *midi);
    qreal bpm() const { return (60000.0 / _bpmPeriodMs); }
signals:
    // Signal emitting a pre-computed pulse-per-quarter-note and quarter-note id (less code in receiver-classes, ie. MinoAnimations)
    void clock(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    // Signal emitting the BPM value changed (useful for UI feedback)
    void bpmChanged(const double bpm);

    // Signal emitting toogling between external and internal clock source
    void useExternalClockSourceChanged(bool);

    // Signals emitting isEnabled changes
    void enabledChanged(bool enabled);
    void disabledChanged(bool disabled);
public slots:
    // Internal generator controls
    void uiTapOn();
    void uiStart();
    void uiStop();
    void uiSync();
    void setBPM(double bpm);

    void setExternalClockSource(bool on);
    bool useExternalClockSource() const { return _useExternalMidiClock; }

private:
    // Current Global Pulse-Per-Quarter-Note Id (in range of [0-384[)
    // 24 ppqn * 16 qn = 384 gppqn
    unsigned int _gppqn;
    unsigned int _uppqn;

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

    // Running status
    bool _isEnabled; // sets when clock source is active.
    void setEnabled(const bool on); // Accessor

    // MIDI
    bool _useExternalMidiClock; // 'true' when clock source comes from external (MIDI clock), 'false' when internal generator is used (Timer)


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
