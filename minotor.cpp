#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>
#include <QGraphicsProxyWidget>
//#include <QRgb>

#include "minoanimation.h"

#include "minochannel.h"

Minotor::Minotor(Midi *midi, QObject *parent) :
    QObject(parent),
    _ledMatrix(NULL),
    _ppqnId(0),
    _isSequenceRunning(false)
{
    _channel1 = new MinoChannel(this);
    _channel2 = new MinoChannel(this);
    _master = new MinoMaster(_channel1, _channel2, this);

//    QRadialGradient gradient(130, 130, 50, 130, 130);
//    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
//    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 1));
//    QBrush brush(gradient);

    _midiInterfaces.append(midi);
    // Connections to Midi manager
    connect(midi,SIGNAL(clockReceived()),this,SLOT(handleClock()));
    connect(midi,SIGNAL(startReceived()),this,SLOT(handleStart()));
    connect(midi,SIGNAL(stopReceived()),this,SLOT(handleStop()));
    connect(midi,SIGNAL(continueReceived()),this,SLOT(handleContinue()));
    connect(midi,SIGNAL(controlChanged(quint8,quint8,quint8)),this,SLOT(handleMidiInterfaceControlChange(quint8,quint8,quint8)));

    // Link Minotor to MidiMapping
    connect(this, SIGNAL(controlChanged(int,quint8,quint8,quint8)), &_midiMapping, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
}

void Minotor::setLedMatrix(LedMatrix *ledMatrix)
{
    _ledMatrix = ledMatrix;
}

void Minotor::animate(const int ppqn)
{
    // Animate channel #1
    channel1()->animate(ppqn);
    // Animate channel #2
    channel2()->animate(ppqn);

    // Render scene to led matrix
    _ledMatrix->showView(master()->view());
}

void Minotor::handleClock(void)
{
    if(_isSequenceRunning)
    {
        if((_ppqnId%2) == 0) {
            animate(_ppqnId);
        }

        // Clock counter
        if (_ppqnId==23) { _ppqnId = 0; } else { _ppqnId++; }
    }
}

void Minotor::handleStop(void)
{
    _isSequenceRunning = false;
}

void Minotor::handleStart(void)
{
    _ppqnId = 0;
    _isSequenceRunning = true;
}

void Minotor::handleContinue(void)
{
    _isSequenceRunning = true;
}

/*
#define KORG_FX_EDIT1 92
void Minotor::handleMidiInterfaceControlChange(quint8 control, quint8 value)
{
    switch (control) {
    case KORG_FX_EDIT1:
        emit colorControlChanged(value);
        break;
    default:
        qDebug() << "unhandled control change: " << control << "( value" << value << ")";
    }
}
*/

void Minotor::handleMidiInterfaceControlChange(quint8 channel, quint8 control, quint8 value)
{
    // qDebug() << "sender:" << QObject::sender()->metaObject()->className();

    Midi *midiInterface = ((Midi*)QObject::sender());
    if (_midiInterfaces.contains(midiInterface))
    {
        const int midiInterfaceId = _midiInterfaces.indexOf(midiInterface);
        emit(controlChanged(midiInterfaceId, channel, control, value));
    } else {
        qDebug() << "Unknow sender";
    }

}
