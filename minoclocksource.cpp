#include "minoclocksource.h"

#include <QDebug>

MinoClockSource::MinoClockSource(QObject *parent) :
    QObject(parent),
    _bpmValuesCount(0),
    _bpmValuesIndex(0),
    _gppqn(0),
    _isMidiSequencerRunning(false)

{
    // BPM Tapping
    _bpmTap.start(); // Note: _bmpTap is always running, tempo values are filtered after

    // Connect internal generator timer to its slot
    connect(&_internalTimer, SIGNAL(timeout()), this, SLOT(internalTimerTimeout()));

    // Set default BPM to 125.8
    _bpmPeriodMs = 60000.0 / 125.8;
    const qreal ms = _bpmPeriodMs / 24;
    qDebug() << "bpm error:" << (qreal)((ms - ((int)ms))*24) << "ms per beat";
    _internalTimer.setInterval(ms);
}

void MinoClockSource::internalTimerTimeout()
{
    _gppqn = (_gppqn + 1)%384;
    emit clock(_gppqn, _gppqn%24, _gppqn/24);
}

void MinoClockSource::uiTapOn()
{
    int ms = _bpmTap.restart();
    qDebug() << "Tap: ms=" << ms << "average ms=" << _bpmAverageMs;

    if ( (ms < 3000) // filter tapping where BPM is less than 20 beat per minute.
         && ((qreal)abs(ms-_bpmAverageMs) < (_bpmAverageMs/2)) // filter tapping where new tap is distant from average at least 50%
         )
    {
        _bpmValues[_bpmValuesIndex] = ms;
        _bpmValuesCount = qMin(10, _bpmValuesCount+1);
        _bpmValuesIndex = (_bpmValuesIndex+1)%10;
        _bpmAverageMs =  _bpmValues[0];
        for(int i=1; i<_bpmValuesCount; i++)
        {
            _bpmAverageMs += _bpmValues[i];
        }
        _bpmAverageMs /= _bpmValuesCount;

        _bpmPeriodMs = _bpmAverageMs;
        const qreal bpm = (1000.0 / _bpmAverageMs) * 60.0;
        qDebug() << "Tap: ms=" << ms << "average ms=" << _bpmAverageMs << "(bpm" << bpm << ")" << "index" << _bpmValuesIndex << "count" << _bpmValuesCount;

        const qreal ms = _bpmPeriodMs / 24;
        qDebug() << "bpm error:" << (qreal)((ms - ((int)ms))*24) << "ms per beat";
        _internalTimer.setInterval(ms);

        emit bpmChanged(bpm);
    } else {
        qDebug() << "Tapping reset";
        _bpmValuesCount = 0;
        _bpmValuesIndex = 0;
        _bpmAverageMs = ms;
    }
}

void MinoClockSource::uiStart()
{
    // Disable Midi Clock source
    _isMidiSequencerRunning = false;
    _internalTimer.start();

    _gppqn = 0;
}

void MinoClockSource::uiStop()
{
    _internalTimer.stop();
}

void MinoClockSource::uiSync()
{
    _gppqn = 0;
}

void MinoClockSource::setMidiClockInterface(Midi *midi)
{
    // Connections to Midi manager
    connect(midi,SIGNAL(clockReceived()),this,SLOT(midiClock()));
    connect(midi,SIGNAL(startReceived()),this,SLOT(midiStart()));
    connect(midi,SIGNAL(stopReceived()),this,SLOT(midiStop()));
    connect(midi,SIGNAL(continueReceived()),this,SLOT(midiContinue()));
}

void MinoClockSource::midiClock()
{
    if(_isMidiSequencerRunning)
    {
        _gppqn = (_gppqn + 1)%384;
        emit clock(_gppqn, _gppqn%24, _gppqn/24);
    }
}

void MinoClockSource::midiStop()
{
    _isMidiSequencerRunning = false;
}

void MinoClockSource::midiStart()
{
    _gppqn = 0;
    _isMidiSequencerRunning = true;

    // Stop internal generator
    _internalTimer.stop();
}

void MinoClockSource::midiContinue()
{
    _isMidiSequencerRunning = true;

    // Stop internal generator
    _internalTimer.stop();
}
