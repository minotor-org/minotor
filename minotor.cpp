#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>

#include "minoprogram.h"
#include "minoanimation.h"

// Animations
#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minaflash.h"
#include "minawaveform.h"
#include "minabarsfromsides.h"
#include "minatext.h"
#include "minafallingobjects.h"
#include "minastars.h"
#include "minaplasma.h"
#include "minagradient.h"
#include "minavibration.h"
#include "minarotatingbars.h"
#include "minacurve.h"

// Programs
#include "miprodebug.h"
#include "miprowaves.h"
#include "miprobnzichru.h"
#include "mipromatrix.h"
#include "miprosecondlives.h"

Minotor::Minotor(QObject *parent) :
    QObject(parent)
{
    // LED Matrix
    _ledMatrix = new LedMatrix(this);

    _master = new MinoMaster(this);
    // Please let MiproDebug as first program:
    //   MiproDebug is design to debug, hence the name, so it should be quick to access.
    //   BTW, MiproDebug can be tweaked depending on current topic..
    _master->setProgram(new MiproDebug(this));

    new MiproSecondLives(this);
    new MiproMatrix(this);
    new MiproBnzIchRU(this);
    new MiproWaves(this);

    // MIDI interfaces
    _midi = new Midi(this);
    connect(_midi, SIGNAL(programChanged(int,quint8,quint8)), this, SLOT(handleMidiInterfaceProgramChange(int,quint8,quint8)));

    // MIDI Mapping
    _midiMapping = new MidiMapping(this);

    // Clock source
    _clockSource = new MinoClockSource(this);
    _clockSource->setMidiClockSource(_midi);
    connect(_clockSource, SIGNAL(clock(unsigned int,unsigned int,unsigned int,unsigned int)), this, SLOT(dispatchClock(unsigned int,unsigned int,unsigned int,unsigned int)));

    // Register HARDCODED triggers notes
    // LPD8
    _midiMapping->mapNoteToRole(1,0,36,"TRANSPORT_PLAY");
    _midiMapping->mapNoteToRole(1,0,37,"TRANSPORT_STOP");
    _midiMapping->mapNoteToRole(1,0,38,"TRANSPORT_SYNC");
    _midiMapping->mapNoteToRole(1,0,39,"TRANSPORT_TAP");

    // Korg nanoKontrol
    _midiMapping->mapControlToRole(2,0,33,"MASTER_ANIMATION_0");
    _midiMapping->mapControlToRole(2,0,34,"MASTER_ANIMATION_1");
    _midiMapping->mapControlToRole(2,0,35,"MASTER_ANIMATION_2");
    _midiMapping->mapControlToRole(2,0,36,"MASTER_ANIMATION_3");
    _midiMapping->mapControlToRole(2,0,37,"MASTER_ANIMATION_4");
    _midiMapping->mapControlToRole(2,0,38,"MASTER_ANIMATION_5");
    _midiMapping->mapControlToRole(2,0,39,"MASTER_ANIMATION_6");
    _midiMapping->mapControlToRole(2,0,40,"MASTER_ANIMATION_7");
    _midiMapping->mapControlToRole(2,0,41,"MASTER_ANIMATION_8");
    _midiMapping->mapControlToRole(2,0,45,"TRANSPORT_PLAY");
    _midiMapping->mapControlToRole(2,0,46,"TRANSPORT_STOP");
    _midiMapping->mapControlToRole(2,0,49,"TRANSPORT_SYNC");
    _midiMapping->mapControlToRole(2,0,44,"TRANSPORT_TAP");

    // Register animations
    MinoAnimationFactory::registerClass<MinaFlash>();
    MinoAnimationFactory::registerClass<MinaExpandingObjects>();
    MinoAnimationFactory::registerClass<MinaWaveform>();
    MinoAnimationFactory::registerClass<MinaVibration>();
    MinoAnimationFactory::registerClass<MinaBarsFromSides>();
    MinoAnimationFactory::registerClass<MinaFallingObjects>();
    MinoAnimationFactory::registerClass<MinaText>();
    MinoAnimationFactory::registerClass<MinaRandomPixels>();
    MinoAnimationFactory::registerClass<MinaStars>();
    MinoAnimationFactory::registerClass<MinaGradient>();
    MinoAnimationFactory::registerClass<MinaPlasma>();
    MinoAnimationFactory::registerClass<MinaRotatingBars>();
    MinoAnimationFactory::registerClass<MinaCurve>();
}

Minotor::~Minotor()
{
    delete _master;
    foreach(MinoProgram *program, programs())
    {
        delete program;
    }

    delete _clockSource;

    delete _midi;

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

void Minotor::handleMidiInterfaceProgramChange(int interface, quint8 channel, quint8 program)
{
    (void)interface;
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

    // Program ID starts at 1
    program->setId(id+1);

    // Inform program about matrix size (used by animations)
    program->setRect(_ledMatrix->rect());

    // Drawing rect
    // On the scene, the program have a dedicated area to display/draw animations
    // This area left one "screen" before and one "screen" areas to prevent from collisions
    // Note: Developer of animations should take care to not collide: its objects should never be larger than one screen-size in all directions (up, down, left, right, diagonals)
    QPointF pos = QPointF(_ledMatrix->size().width()*3, _ledMatrix->size().height() + ((_ledMatrix->size().height()*3) * id));
    program->setDrawingPos(pos);
}
