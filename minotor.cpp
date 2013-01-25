#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>

#include "minochannel.h"
#include "minoanimation.h"

Minotor::Minotor(QObject *parent) :
    QObject(parent)
{
    const QSize channelSize(24, 16);
    _channel1 = new MinoChannel(channelSize, this);
    _channel2 = new MinoChannel(channelSize, this);
    _master = new MinoMaster(_channel1, _channel2, this);

    // LED Matrix
    _ledMatrix = new LedMatrix();

    // MIDI interfaces
    Midi *midi = new Midi();
    _midiInterfaces.append(midi);
    connect(midi,SIGNAL(controlChanged(quint8,quint8,quint8)),this,SLOT(handleMidiInterfaceControlChange(quint8,quint8,quint8)));

    // Link Minotor to MidiMapping
    connect(this, SIGNAL(controlChanged(int,quint8,quint8,quint8)), &_midiMapping, SLOT(midiControlChanged(int,quint8,quint8,quint8)));

    // Clock source
    _clockSource = new MinoClockSource(this);
    connect(_clockSource, SIGNAL(clock(int,int)), this, SLOT(dispatchClock(int,int)));
}

void Minotor::dispatchClock(const int ppqn, const int qn)
{
    if((ppqn%2) == 0) {
        // Animate channel #1
        channel1()->animate(ppqn);
        // Animate channel #2
        channel2()->animate(ppqn);

        // Render scene to led matrix
        _ledMatrix->showView(master()->view());
    }
}

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
