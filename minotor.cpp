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
#include "minaballs.h"
#include "minaimage.h"

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
    _midiMapper = new MidiMapper(this);

    // Clock source
    _clockSource = new MinoClockSource(this);
    _clockSource->setMidiClockSource(_midi);
    connect(_clockSource, SIGNAL(clock(unsigned int,unsigned int,unsigned int,unsigned int)), this, SLOT(dispatchClock(unsigned int,unsigned int,unsigned int,unsigned int)));

    // Register HARDCODED triggers notes
/*
    // LPD8
    _midiMapper->mapNoteToRole(1,0,36,"TRANSPORT_PLAY");
    _midiMapper->mapNoteToRole(1,0,37,"TRANSPORT_STOP");
    _midiMapper->mapNoteToRole(1,0,38,"TRANSPORT_SYNC");
    _midiMapper->mapNoteToRole(1,0,39,"TRANSPORT_TAP");
*/
/*
    // Korg nanoKontrol
    _midiMapper->mapControlToRole(2,0,14,"MASTER_CONTROLS_0_0");
    _midiMapper->mapControlToRole(2,0,15,"MASTER_CONTROLS_1_0");
    _midiMapper->mapControlToRole(2,0,16,"MASTER_CONTROLS_2_0");
    _midiMapper->mapControlToRole(2,0,17,"MASTER_CONTROLS_3_0");
    _midiMapper->mapControlToRole(2,0,18,"MASTER_CONTROLS_4_0");
    _midiMapper->mapControlToRole(2,0,19,"MASTER_CONTROLS_5_0");
    _midiMapper->mapControlToRole(2,0,20,"MASTER_CONTROLS_6_0");
    _midiMapper->mapControlToRole(2,0,21,"MASTER_CONTROLS_7_0");
    _midiMapper->mapControlToRole(2,0,22,"MASTER_CONTROLS_8_0");

    _midiMapper->mapControlToRole(2,0,2,"MASTER_CONTROLS_0_1");
    _midiMapper->mapControlToRole(2,0,3,"MASTER_CONTROLS_1_1");
    _midiMapper->mapControlToRole(2,0,4,"MASTER_CONTROLS_2_1");
    _midiMapper->mapControlToRole(2,0,5,"MASTER_CONTROLS_3_1");
    _midiMapper->mapControlToRole(2,0,6,"MASTER_CONTROLS_4_1");
    _midiMapper->mapControlToRole(2,0,8,"MASTER_CONTROLS_5_1");
    _midiMapper->mapControlToRole(2,0,9,"MASTER_CONTROLS_6_1");
    _midiMapper->mapControlToRole(2,0,12,"MASTER_CONTROLS_7_1");
    _midiMapper->mapControlToRole(2,0,13,"MASTER_CONTROLS_8_1");

    _midiMapper->mapControlToRole(2,0,33,"MASTER_ANIMATION_0");
    _midiMapper->mapControlToRole(2,0,34,"MASTER_ANIMATION_1");
    _midiMapper->mapControlToRole(2,0,35,"MASTER_ANIMATION_2");
    _midiMapper->mapControlToRole(2,0,36,"MASTER_ANIMATION_3");
    _midiMapper->mapControlToRole(2,0,37,"MASTER_ANIMATION_4");
    _midiMapper->mapControlToRole(2,0,38,"MASTER_ANIMATION_5");
    _midiMapper->mapControlToRole(2,0,39,"MASTER_ANIMATION_6");
    _midiMapper->mapControlToRole(2,0,40,"MASTER_ANIMATION_7");
    _midiMapper->mapControlToRole(2,0,41,"MASTER_ANIMATION_8");

    _midiMapper->mapControlToRole(2,0,23,"MASTER_ANIMATION_SHIFT_0");
    _midiMapper->mapControlToRole(2,0,24,"MASTER_ANIMATION_SHIFT_1");
    _midiMapper->mapControlToRole(2,0,25,"MASTER_ANIMATION_SHIFT_2");
    _midiMapper->mapControlToRole(2,0,26,"MASTER_ANIMATION_SHIFT_3");
    _midiMapper->mapControlToRole(2,0,27,"MASTER_ANIMATION_SHIFT_4");
    _midiMapper->mapControlToRole(2,0,28,"MASTER_ANIMATION_SHIFT_5");
    _midiMapper->mapControlToRole(2,0,29,"MASTER_ANIMATION_SHIFT_6");
    _midiMapper->mapControlToRole(2,0,30,"MASTER_ANIMATION_SHIFT_7");
    _midiMapper->mapControlToRole(2,0,31,"MASTER_ANIMATION_SHIFT_8");

    _midiMapper->mapControlToRole(2,0,45,"TRANSPORT_PLAY");
    _midiMapper->mapControlToRole(2,0,46,"TRANSPORT_STOP");
    _midiMapper->mapControlToRole(2,0,49,"TRANSPORT_SYNC");
    _midiMapper->mapControlToRole(2,0,44,"TRANSPORT_TAP");
*/

    // Behringer BCD3000
    _midiMapper->mapControlToRole(1,0,3,"MASTER_CONTROLS_0_0");
    _midiMapper->mapControlToRole(1,0,4,"MASTER_CONTROLS_1_0");
    _midiMapper->mapControlToRole(1,0,5,"MASTER_CONTROLS_2_0");
    _midiMapper->mapControlToRole(1,0,6,"MASTER_CONTROLS_3_0");
    _midiMapper->mapControlToRole(1,0,10,"MASTER_CONTROLS_4_0");
    _midiMapper->mapControlToRole(1,0,7,"MASTER_CONTROLS_5_0");
    _midiMapper->mapControlToRole(1,0,8,"MASTER_CONTROLS_6_0");
    _midiMapper->mapControlToRole(1,0,9,"MASTER_CONTROLS_7_0");

    _midiMapper->mapNoteToRole(1,0,12,"MASTER_ANIMATION_SHIFT_0");
    _midiMapper->mapNoteToRole(1,0,13,"MASTER_ANIMATION_SHIFT_1");
    _midiMapper->mapNoteToRole(1,0,14,"MASTER_ANIMATION_SHIFT_2");
    _midiMapper->mapNoteToRole(1,0,4,"MASTER_ANIMATION_SHIFT_3");
    _midiMapper->mapNoteToRole(1,0,10,"MASTER_ANIMATION_SHIFT_4");
    _midiMapper->mapNoteToRole(1,0,20,"MASTER_ANIMATION_SHIFT_5");
    _midiMapper->mapNoteToRole(1,0,21,"MASTER_ANIMATION_SHIFT_6");
    _midiMapper->mapNoteToRole(1,0,22,"MASTER_ANIMATION_SHIFT_7");

    _midiMapper->mapNoteToRole(1,0,26,"TRANSPORT_PLAY");
    _midiMapper->mapNoteToRole(1,0,27,"TRANSPORT_TAP");

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
    MinoAnimationFactory::registerClass<MinaBalls>();
    MinoAnimationFactory::registerClass<MinaImage>();
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

void Minotor::save(MinoPersistentObject* object, QSettings* parser)
{
    if(object)
    {
        // Start a group using classname
        parser->beginGroup(object->metaObject()->className());
        // Remove all entries in this group
        parser->remove("");
        qDebug() << QString(" ").repeated(2) << object;

        // Start an array of properties
        parser->beginWriteArray("properties");
        parser->remove("");

        for(int j=0; j<object->metaObject()->propertyCount(); j++)
        {
            parser->setArrayIndex(j);
            QMetaProperty omp = object->metaObject()->property(j);
            parser->setValue(omp.name(), omp.read(object));
            qDebug() << QString(" ").repeated(3)
                     << omp.typeName()
                     << omp.name()
                     << omp.read(object)
                     << omp.isStored();
        }
        // End of properties array
        parser->endArray();

        parser->beginWriteArray("children");
        parser->remove("");
        QObjectList ol = object->children();
        int j = 0;
        foreach(QObject* o, ol)
        {
            if(MinoPersistentObject* mpo = qobject_cast<MinoPersistentObject*>(o))
            {
                parser->setArrayIndex(j++);
                save(mpo, parser);
            }
        }
        parser->endArray();
        parser->endGroup();
    }
}
