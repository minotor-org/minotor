#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>

#include "minochannel.h"
#include "minoanimation.h"

#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minadebug.h"
#include "minawaveform.h"
#include "minabarsfromsides.h"

Minotor::Minotor(QObject *parent) :
    QObject(parent)
{
    // LED Matrix
    _ledMatrix = new LedMatrix(this);

    // Choose appropriated drawing rect:
    //   Both channels share the same scene, so be sure they don't collide...
    const QRect masterDrawingRect(QRect(100, 0, 24, 16));
    const QRect cueDrawingRect(QRect(50, 0, 24, 16));

    _master = new MinoMaster(this, masterDrawingRect);
    _cue = new MinoCue(this, cueDrawingRect);

    // MIDI interfaces
    Midi *midi = new Midi(this);
    _midiInterfaces.append(midi);
    connect(midi,SIGNAL(controlChanged(quint8,quint8,quint8)),this,SLOT(handleMidiInterfaceControlChange(quint8,quint8,quint8)));

    // Link Minotor to MidiMapping
    connect(this, SIGNAL(controlChanged(int,quint8,quint8,quint8)), &_midiMapping, SLOT(midiControlChanged(int,quint8,quint8,quint8)));

    // Clock source
    _clockSource = new MinoClockSource(this);
    connect(_clockSource, SIGNAL(clock(unsigned int,unsigned int,unsigned int)), this, SLOT(dispatchClock(unsigned int,unsigned int,unsigned int)));

    // Register animations
    MinoAnimationFactory::registerClass<MinaDebug>();
    MinoAnimationFactory::registerClass<MinaExpandingObjects>();
    MinoAnimationFactory::registerClass<MinaBarsFromSides>();
    MinoAnimationFactory::registerClass<MinaRandomPixels>();
    MinoAnimationFactory::registerClass<MinaWaveform>();
}

Minotor::~Minotor()
{
    delete _clockSource;

    delete _cue;
    delete _master;

    delete _ledMatrix;
    foreach (Midi *midi, _midiInterfaces)
    {
        delete midi;
    }
}

void Minotor::dispatchClock(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    if((ppqn%2) == 0) {
        // Animate master
        _master->animate(gppqn, ppqn, qn);

        // Render scene to led matrix
        _ledMatrix->show(master()->rendering());

        // Animate cue
        _cue->animate(gppqn, ppqn, qn);
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
