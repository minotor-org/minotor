#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>

#include "minoprogram.h"
#include "minoanimation.h"

#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minaflash.h"
#include "minawaveform.h"
#include "minabarsfromsides.h"
#include "minatext.h"
#include "miprowaves.h"
#include "miprotext.h"
#include "minafallingobjects.h"

Minotor::Minotor(QObject *parent) :
    QObject(parent)
{
    // LED Matrix
    _ledMatrix = new LedMatrix(this);

    _master = new MinoMaster(this);
    _master->setProgram(new MiproText(this));
    new MiproWaves(this);
    new MiproMatrix(this);
    // MIDI interfaces
    Midi *midi = new Midi(this);
    _midiInterfaces.append(midi);
    connect(midi,SIGNAL(controlChanged(quint8,quint8,quint8)),this,SLOT(handleMidiInterfaceControlChange(quint8,quint8,quint8)));
    connect(midi, SIGNAL(programChanged(quint8,quint8)), this, SLOT(handleMidiInterfaceProgramChange(quint8,quint8)));
    connect(midi, SIGNAL(noteChanged(quint8,quint8,bool,quint8)), _master, SLOT(noteChanged(quint8,quint8,bool,quint8)));

    // Link Minotor to MidiMapping
    connect(this, SIGNAL(controlChanged(int,quint8,quint8,quint8)), &_midiMapping, SLOT(midiControlChanged(int,quint8,quint8,quint8)));

    // Clock source
    _clockSource = new MinoClockSource(this);
    _clockSource->setMidiClockInterface(midi);
    connect(_clockSource, SIGNAL(clock(unsigned int,unsigned int,unsigned int,unsigned int)), this, SLOT(dispatchClock(unsigned int,unsigned int,unsigned int,unsigned int)));

    // Register animations
    MinoAnimationFactory::registerClass<MinaFlash>();
    MinoAnimationFactory::registerClass<MinaExpandingObjects>();
    MinoAnimationFactory::registerClass<MinaBarsFromSides>();
    MinoAnimationFactory::registerClass<MinaRandomPixels>();
    MinoAnimationFactory::registerClass<MinaWaveform>();
    MinoAnimationFactory::registerClass<MinaText>();
    MinoAnimationFactory::registerClass<MinaFallingObjects>();
}

Minotor::~Minotor()
{
    delete _clockSource;

    foreach (Midi *midi, _midiInterfaces)
    {
        delete midi;
    }

    delete _master;

    delete _ledMatrix;
}

void Minotor::dispatchClock(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    if((ppqn%2) == 0) {
        // Animate master
        _master->program()->animate(uppqn, gppqn, ppqn, qn);

        // Render scene to led matrix
        _ledMatrix->show(master()->program()->rendering());

        for(int i=0; i<_programs.count(); i++)
        {
            MinoProgram *program = _programs.at(i);
            if (program!=_master->program())
            {
                if(program->isSelected())
                {
                    program->animate(uppqn, gppqn, ppqn, qn);
                }
            }
        }
        if (ppqn == 0)
        {
            emit(beatToggled(true));
        } else if (ppqn == 2)
        {
            emit(beatToggled(false));
        }
    }
}

void Minotor::handleMidiInterfaceControlChange(quint8 program, quint8 control, quint8 value)
{
    // qDebug() << "sender:" << QObject::sender()->metaObject()->className();

    Midi *midiInterface = ((Midi*)QObject::sender());
    if (_midiInterfaces.contains(midiInterface))
    {
        const int midiInterfaceId = _midiInterfaces.indexOf(midiInterface);
        emit(controlChanged(midiInterfaceId, program, control, value));
    } else {
        qDebug() << "Unknow sender";
    }
}

void Minotor::handleMidiInterfaceProgramChange(quint8 channel, quint8 program)
{
    (void)channel;
    if(program < _programs.count())
    {
        _master->setProgram(_programs.at(program));
    }
}

void Minotor::addProgram(MinoProgram *program)
{
    _programs.append(program);
    const int id = _programs.indexOf(program);
    program->setId(id);

    program->setRect(_ledMatrix->rect());

    // Drawing rect
    QPointF pos = QPointF(_ledMatrix->size().width() + 100, (_ledMatrix->size().height() + 100) * id);
    program->setDrawingPos(pos);
}
