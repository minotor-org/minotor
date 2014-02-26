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

#include "minotor.h"

#include <QtCore/QDebug>

#include <QGraphicsView>
#include <QPainter>

#include "minoprogram.h"
#include "minopersistentobjectfactory.h"

// Animations
#include "minoanimation.h"
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
#include "minaflashbars.h"
#include "minarainbowoil.h"
#include "minagrid.h"

// Programs
#include "miprodebug.h"
#include "miprowaves.h"
#include "miprobnzichru.h"
#include "mipromatrix.h"
#include "miprosecondlives.h"

Minotor::Minotor(QObject *parent) :
    QObject(parent)
{
    // Settings (ini file to keep local user profile: rendering size, MIDI interfaces, LED matrix, etc.)
    _settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, QString("Minotor"));
    // Settings: load renderer size
    _panelSize = _settings->value("renderer/panelSize").toSize();
    if(!_panelSize.isValid())
        _panelSize = QSize(8, 8);
    _matrixSize = _settings->value("renderer/matrixSize").toSize();
    if(!_matrixSize.isValid())
        _matrixSize = QSize(3, 2);
    _rendererSize = _settings->value("renderer/size").toSize();
    if(!_rendererSize.isValid())
        _rendererSize = QSize(24, 16);

    // Master
    _master = new MinoMaster(this);

    // LED Matrix
    // NOTE: ATM LedMatrix size must be equal to rendering size.
    _ledMatrix = new LedMatrix(_rendererSize,_panelSize, _matrixSize, this);
    // Program Bank
    _programBank = new MinoProgramBank(this);

    // MIDI interfaces
    _midi = new Midi(this);
    connect(_midi, SIGNAL(programChanged(int,quint8,quint8)), this, SLOT(handleMidiInterfaceProgramChange(int,quint8,quint8)));

    // MIDI Mapping
    _midiMapper = new MidiMapper(this);

    // Clock source
    _clockSource = new MinoClockSource(this);
    _clockSource->setMidiClockSource(_midi);
    connect(_clockSource, SIGNAL(clock(uint,uint,uint,uint)), this, SLOT(dispatchClock(uint,uint,uint,uint)));

    // Register animations
    MinoPersistentObjectFactory::registerAnimationClass<MinaFlash>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaExpandingObjects>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaWaveform>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaVibration>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaBarsFromSides>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaFallingObjects>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaText>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaRandomPixels>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaStars>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaGradient>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaPlasma>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaRotatingBars>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaCurve>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaBalls>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaImage>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaFlashBars>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaRainbowOil>();
    MinoPersistentObjectFactory::registerAnimationClass<MinaGrid>();

    // Register other instantiable persistent objects
    MinoPersistentObjectFactory::registerClass<MinoProgramBank>();
    MinoPersistentObjectFactory::registerClass<MinoProgram>();
    MinoPersistentObjectFactory::registerClass<MinoAnimationGroup>();

}

void Minotor::loadSettings()
{
    // Setup LedMatrix, MIDI, etc. from configuration file
    // NOTE: Renderer size have already been proceeded
    loadMidiSettings();
    loadLedMatrixSettings();
}

void Minotor::initWithDebugSetup()
{
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
    // Knobs (scene 1)
    _midiMapper->mapControlToRole(0,0,14,"MASTER_CONTROLS_0_0");
    _midiMapper->mapControlToRole(0,0,15,"MASTER_CONTROLS_1_0");
    _midiMapper->mapControlToRole(0,0,16,"MASTER_CONTROLS_2_0");
    _midiMapper->mapControlToRole(0,0,17,"MASTER_CONTROLS_3_0");
    _midiMapper->mapControlToRole(0,0,18,"MASTER_CONTROLS_4_0");
    _midiMapper->mapControlToRole(0,0,19,"MASTER_CONTROLS_5_0");
    _midiMapper->mapControlToRole(0,0,20,"MASTER_CONTROLS_6_0");
    _midiMapper->mapControlToRole(0,0,21,"MASTER_CONTROLS_7_0");
    _midiMapper->mapControlToRole(0,0,22,"MASTER_CONTROLS_8_0");
    // Knobs (scene 2)
    _midiMapper->mapControlToRole(0,0,57,"MASTER_CONTROLS_9_0");
    _midiMapper->mapControlToRole(0,0,58,"MASTER_CONTROLS_10_0");
    _midiMapper->mapControlToRole(0,0,59,"MASTER_CONTROLS_11_0");
    _midiMapper->mapControlToRole(0,0,60,"MASTER_CONTROLS_12_0");
    _midiMapper->mapControlToRole(0,0,61,"MASTER_CONTROLS_13_0");
    _midiMapper->mapControlToRole(0,0,62,"MASTER_CONTROLS_14_0");
    _midiMapper->mapControlToRole(0,0,63,"MASTER_CONTROLS_15_0");
    _midiMapper->mapControlToRole(0,0,65,"MASTER_CONTROLS_16_0");
    _midiMapper->mapControlToRole(0,0,66,"MASTER_CONTROLS_17_0");
    // Knobs (scene 3)
    _midiMapper->mapControlToRole(0,0,94,"MASTER_CONTROLS_18_0");
    _midiMapper->mapControlToRole(0,0,95,"MASTER_CONTROLS_19_0");
    _midiMapper->mapControlToRole(0,0,96,"MASTER_CONTROLS_20_0");
    _midiMapper->mapControlToRole(0,0,97,"MASTER_CONTROLS_21_0");
    _midiMapper->mapControlToRole(0,0,102,"MASTER_CONTROLS_22_0");
    _midiMapper->mapControlToRole(0,0,103,"MASTER_CONTROLS_23_0");
    _midiMapper->mapControlToRole(0,0,104,"MASTER_CONTROLS_24_0");
    _midiMapper->mapControlToRole(0,0,105,"MASTER_CONTROLS_25_0");
    _midiMapper->mapControlToRole(0,0,106,"MASTER_CONTROLS_26_0");
    // Knobs (scene 4)
    _midiMapper->mapControlToRole(0,0,10,"MASTER_CONTROLS_27_0");
    _midiMapper->mapControlToRole(0,1,10,"MASTER_CONTROLS_28_0");
    _midiMapper->mapControlToRole(0,2,10,"MASTER_CONTROLS_29_0");
    _midiMapper->mapControlToRole(0,3,10,"MASTER_CONTROLS_30_0");
    _midiMapper->mapControlToRole(0,4,10,"MASTER_CONTROLS_31_0");
    _midiMapper->mapControlToRole(0,5,10,"MASTER_CONTROLS_32_0");
    _midiMapper->mapControlToRole(0,6,10,"MASTER_CONTROLS_33_0");
    _midiMapper->mapControlToRole(0,7,10,"MASTER_CONTROLS_34_0");
    _midiMapper->mapControlToRole(0,8,10,"MASTER_CONTROLS_35_0");
     // Sliders (scene 1)
    _midiMapper->mapControlToRole(0,0,2,"MASTER_CONTROLS_0_1");
    _midiMapper->mapControlToRole(0,0,3,"MASTER_CONTROLS_1_1");
    _midiMapper->mapControlToRole(0,0,4,"MASTER_CONTROLS_2_1");
    _midiMapper->mapControlToRole(0,0,5,"MASTER_CONTROLS_3_1");
    _midiMapper->mapControlToRole(0,0,6,"MASTER_CONTROLS_4_1");
    _midiMapper->mapControlToRole(0,0,8,"MASTER_CONTROLS_5_1");
    _midiMapper->mapControlToRole(0,0,9,"MASTER_CONTROLS_6_1");
    _midiMapper->mapControlToRole(0,0,12,"MASTER_CONTROLS_7_1");
    _midiMapper->mapControlToRole(0,0,13,"MASTER_CONTROLS_8_1");
    // Sliders (scene 2)
    _midiMapper->mapControlToRole(0,0,42,"MASTER_CONTROLS_9_1");
    _midiMapper->mapControlToRole(0,0,43,"MASTER_CONTROLS_10_1");
    _midiMapper->mapControlToRole(0,0,50,"MASTER_CONTROLS_11_1");
    _midiMapper->mapControlToRole(0,0,51,"MASTER_CONTROLS_12_1");
    _midiMapper->mapControlToRole(0,0,52,"MASTER_CONTROLS_13_1");
    _midiMapper->mapControlToRole(0,0,53,"MASTER_CONTROLS_14_1");
    _midiMapper->mapControlToRole(0,0,54,"MASTER_CONTROLS_15_1");
    _midiMapper->mapControlToRole(0,0,55,"MASTER_CONTROLS_16_1");
    _midiMapper->mapControlToRole(0,0,56,"MASTER_CONTROLS_17_1");
    // Sliders (scene 3)
    _midiMapper->mapControlToRole(0,0,85,"MASTER_CONTROLS_18_1");
    _midiMapper->mapControlToRole(0,0,86,"MASTER_CONTROLS_19_1");
    _midiMapper->mapControlToRole(0,0,87,"MASTER_CONTROLS_20_1");
    _midiMapper->mapControlToRole(0,0,88,"MASTER_CONTROLS_21_1");
    _midiMapper->mapControlToRole(0,0,89,"MASTER_CONTROLS_22_1");
    _midiMapper->mapControlToRole(0,0,90,"MASTER_CONTROLS_23_1");
    _midiMapper->mapControlToRole(0,0,91,"MASTER_CONTROLS_24_1");
    _midiMapper->mapControlToRole(0,0,92,"MASTER_CONTROLS_25_1");
    _midiMapper->mapControlToRole(0,0,93,"MASTER_CONTROLS_26_1");
    // Sliders (scene 4)
    _midiMapper->mapControlToRole(0,0,7,"MASTER_CONTROLS_27_1");
    _midiMapper->mapControlToRole(0,1,7,"MASTER_CONTROLS_28_1");
    _midiMapper->mapControlToRole(0,2,7,"MASTER_CONTROLS_29_1");
    _midiMapper->mapControlToRole(0,3,7,"MASTER_CONTROLS_30_1");
    _midiMapper->mapControlToRole(0,4,7,"MASTER_CONTROLS_31_1");
    _midiMapper->mapControlToRole(0,5,7,"MASTER_CONTROLS_32_1");
    _midiMapper->mapControlToRole(0,6,7,"MASTER_CONTROLS_33_1");
    _midiMapper->mapControlToRole(0,7,7,"MASTER_CONTROLS_34_1");
    _midiMapper->mapControlToRole(0,8,7,"MASTER_CONTROLS_35_1");
    // First lines buttons (scene 1)
    _midiMapper->mapControlToRole(0,0,23,"MASTER_ANIMATION_SHIFT_0");
    _midiMapper->mapControlToRole(0,0,24,"MASTER_ANIMATION_SHIFT_1");
    _midiMapper->mapControlToRole(0,0,25,"MASTER_ANIMATION_SHIFT_2");
    _midiMapper->mapControlToRole(0,0,26,"MASTER_ANIMATION_SHIFT_3");
    _midiMapper->mapControlToRole(0,0,27,"MASTER_ANIMATION_SHIFT_4");
    _midiMapper->mapControlToRole(0,0,28,"MASTER_ANIMATION_SHIFT_5");
    _midiMapper->mapControlToRole(0,0,29,"MASTER_ANIMATION_SHIFT_6");
    _midiMapper->mapControlToRole(0,0,30,"MASTER_ANIMATION_SHIFT_7");
    _midiMapper->mapControlToRole(0,0,31,"MASTER_ANIMATION_SHIFT_8");
    // First lines buttons (scene 2)
    _midiMapper->mapControlToRole(0,0,67,"MASTER_ANIMATION_SHIFT_9");
    _midiMapper->mapControlToRole(0,0,68,"MASTER_ANIMATION_SHIFT_10");
    _midiMapper->mapControlToRole(0,0,69,"MASTER_ANIMATION_SHIFT_11");
    _midiMapper->mapControlToRole(0,0,70,"MASTER_ANIMATION_SHIFT_12");
    _midiMapper->mapControlToRole(0,0,71,"MASTER_ANIMATION_SHIFT_13");
    _midiMapper->mapControlToRole(0,0,72,"MASTER_ANIMATION_SHIFT_14");
    _midiMapper->mapControlToRole(0,0,73,"MASTER_ANIMATION_SHIFT_15");
    _midiMapper->mapControlToRole(0,0,74,"MASTER_ANIMATION_SHIFT_16");
    _midiMapper->mapControlToRole(0,0,75,"MASTER_ANIMATION_SHIFT_17");
    // First lines buttons (scene 3)
    _midiMapper->mapControlToRole(0,0,107,"MASTER_ANIMATION_SHIFT_18");
    _midiMapper->mapControlToRole(0,0,108,"MASTER_ANIMATION_SHIFT_19");
    _midiMapper->mapControlToRole(0,0,109,"MASTER_ANIMATION_SHIFT_20");
    _midiMapper->mapControlToRole(0,0,110,"MASTER_ANIMATION_SHIFT_21");
    _midiMapper->mapControlToRole(0,0,111,"MASTER_ANIMATION_SHIFT_22");
    _midiMapper->mapControlToRole(0,0,112,"MASTER_ANIMATION_SHIFT_23");
    _midiMapper->mapControlToRole(0,0,113,"MASTER_ANIMATION_SHIFT_24");
    _midiMapper->mapControlToRole(0,0,114,"MASTER_ANIMATION_SHIFT_25");
    _midiMapper->mapControlToRole(0,0,115,"MASTER_ANIMATION_SHIFT_26");
    // First lines buttons (scene 4)
    // The default behavior on this device does not allow to use the #4 scene
    // Default configuration act as toggle switch but does not remains status when changing scene (4 -> 0 -> ... -> 4)

    // Second lines buttons (scene 1)
    _midiMapper->mapControlToRole(0,0,33,"MASTER_ANIMATION_0");
    _midiMapper->mapControlToRole(0,0,34,"MASTER_ANIMATION_1");
    _midiMapper->mapControlToRole(0,0,35,"MASTER_ANIMATION_2");
    _midiMapper->mapControlToRole(0,0,36,"MASTER_ANIMATION_3");
    _midiMapper->mapControlToRole(0,0,37,"MASTER_ANIMATION_4");
    _midiMapper->mapControlToRole(0,0,38,"MASTER_ANIMATION_5");
    _midiMapper->mapControlToRole(0,0,39,"MASTER_ANIMATION_6");
    _midiMapper->mapControlToRole(0,0,40,"MASTER_ANIMATION_7");
    _midiMapper->mapControlToRole(0,0,41,"MASTER_ANIMATION_8");
    // Second lines buttons (scene 2)
    _midiMapper->mapControlToRole(0,0,76,"MASTER_ANIMATION_9");
    _midiMapper->mapControlToRole(0,0,77,"MASTER_ANIMATION_10");
    _midiMapper->mapControlToRole(0,0,78,"MASTER_ANIMATION_11");
    _midiMapper->mapControlToRole(0,0,79,"MASTER_ANIMATION_12");
    _midiMapper->mapControlToRole(0,0,80,"MASTER_ANIMATION_13");
    _midiMapper->mapControlToRole(0,0,81,"MASTER_ANIMATION_14");
    _midiMapper->mapControlToRole(0,0,82,"MASTER_ANIMATION_15");
    _midiMapper->mapControlToRole(0,0,83,"MASTER_ANIMATION_16");
    _midiMapper->mapControlToRole(0,0,84,"MASTER_ANIMATION_17");
    // Second lines buttons (scene 3)
    _midiMapper->mapControlToRole(0,0,116,"MASTER_ANIMATION_18");
    _midiMapper->mapControlToRole(0,0,117,"MASTER_ANIMATION_19");
    _midiMapper->mapControlToRole(0,0,118,"MASTER_ANIMATION_20");
    _midiMapper->mapControlToRole(0,0,119,"MASTER_ANIMATION_21");
    _midiMapper->mapControlToRole(0,0,120,"MASTER_ANIMATION_22");
    _midiMapper->mapControlToRole(0,0,121,"MASTER_ANIMATION_23");
    _midiMapper->mapControlToRole(0,0,122,"MASTER_ANIMATION_24");
    _midiMapper->mapControlToRole(0,0,123,"MASTER_ANIMATION_25");
    _midiMapper->mapControlToRole(0,0,124,"MASTER_ANIMATION_26");

    _midiMapper->mapControlToRole(0,0,45,"TRANSPORT_PLAY");
    _midiMapper->mapControlToRole(0,0,49,"TRANSPORT_SYNC");
    _midiMapper->mapControlToRole(0,0,44,"TRANSPORT_TAP");

    _midiMapper->mapControlToRole(0,0,47,"BANK_PREVIOUS");
    _midiMapper->mapControlToRole(0,0,48,"BANK_NEXT");
    _midiMapper->mapControlToRole(0,0,46,"BANK_SELECT");

    // Piano
    _midiMapper->mapNoteToRole(1,0,59,"MASTER_ANIMATION_0");
    _midiMapper->mapNoteToRole(1,0,60,"MASTER_ANIMATION_1");
    _midiMapper->mapNoteToRole(1,0,61,"MASTER_ANIMATION_2");
    _midiMapper->mapNoteToRole(1,0,62,"MASTER_ANIMATION_3");
    _midiMapper->mapNoteToRole(1,0,63,"MASTER_ANIMATION_4");
    _midiMapper->mapNoteToRole(1,0,64,"MASTER_ANIMATION_5");
    _midiMapper->mapNoteToRole(1,0,65,"MASTER_ANIMATION_6");
    _midiMapper->mapNoteToRole(1,0,66,"MASTER_ANIMATION_7");
    _midiMapper->mapNoteToRole(1,0,67,"MASTER_ANIMATION_8");
    _midiMapper->mapNoteToRole(1,0,68,"MASTER_ANIMATION_9");
    _midiMapper->mapNoteToRole(1,0,69,"MASTER_ANIMATION_10");
    _midiMapper->mapNoteToRole(1,0,70,"MASTER_ANIMATION_11");
    _midiMapper->mapNoteToRole(1,0,71,"MASTER_ANIMATION_12");
    _midiMapper->mapNoteToRole(1,0,72,"MASTER_ANIMATION_13");
    _midiMapper->mapNoteToRole(1,0,73,"MASTER_ANIMATION_14");
    _midiMapper->mapNoteToRole(1,0,74,"MASTER_ANIMATION_15");
    _midiMapper->mapNoteToRole(1,0,75,"MASTER_ANIMATION_16");
    _midiMapper->mapNoteToRole(1,0,76,"MASTER_ANIMATION_17");
    _midiMapper->mapNoteToRole(1,0,77,"MASTER_ANIMATION_18");
*/
/*
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
*/

    // Please let MiproDebug as first program:
    //   MiproDebug is design to debug, hence the name, so it should be quick to access.
    //   BTW, MiproDebug can be tweaked depending on current topic..
    _master->setProgram(new MiproDebug(_programBank));
//    new MiproSecondLives(_programBank);
//    new MiproMatrix(_programBank);
//    new MiproBnzIchRU(_programBank);
//    new MiproWaves(_programBank);
/*
    MinoProgram* prg1 = new MiproSecondLives(_programBank);
    //_programBank->addProgram(new MiproSecondLives(_programBank));
    //_programBank->addProgram(new MiproMatrix());
    //_programBank->addProgram(new MiproBnzIchRU());
    //_programBank->addProgram(new MiproWaves());
    _master->setProgram(_programBank->programs().at(0));
*/
}

Minotor::~Minotor()
{
    delete _settings;

    delete _master;

    delete _programBank;

    delete _clockSource;

    delete _midi;

    delete _ledMatrix;
}

void Minotor::dispatchClock(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    if((ppqn%2) == 0) {
        // Animate master
        if(_master->program())
        {
            _master->program()->animate(uppqn, gppqn, ppqn, qn);

            // Render scene to led matrix
            _ledMatrix->show(master()->program()->rendering());
        }

        QList<MinoProgram*> programs = _programBank->programs();
        for(int i=0; i<programs.count(); i++)
        {
            MinoProgram *program = programs.at(i);
            if (program!=_master->program())
            {
                if(program->isSelected())
                {
                    program->animate(uppqn, gppqn, ppqn, qn);
                }
            }
        }
    }
}

void Minotor::handleMidiInterfaceProgramChange(int interface, quint8 channel, quint8 program)
{
    (void)interface;
    (void)channel;
    /*
    QList<MinoProgram*> programs = _programBank->programs();
    if(program < programs.count())
    {
        _master->setProgram(programs.at(program));
    }*/
    // HACK to use Korg scene button
    const int size = 9;
    _master->setViewportRange(program*size, ((program+1)*size)-1);
}

void Minotor::loadMidiSettings()
{
    // MIDI
    _settings->beginGroup("midi");

    _settings->beginGroup("interface");
    foreach(const QString& group, _settings->childGroups())
    {
        // Use id as group in settings (ie: midi/interface/0)
        _settings->beginGroup(group);

        if(!_settings->contains("name"))
        {
            qDebug() << Q_FUNC_INFO
                     << "Error while loading: midi interface name";
            break;
        }
        const QString midiPort = _settings->value("name").toString();
        MidiInterface *midiInterface = _midi->interface(midiPort);
        if(!midiInterface)
        {
            // Midi interface is not detected, lets create it!
            midiInterface = _midi->addMidiInterface(midiPort);
        }
        if(midiInterface)
        {
            midiInterface->setId(group.toInt());

            QObject *object = static_cast<QObject*>(midiInterface);
            for(int j=0; j<object->metaObject()->propertyCount(); j++)
            {
                QMetaProperty omp = object->metaObject()->property(j);
                // "name" key has already been used
                if((omp.name() != QString("name")) && (omp.name() != QString("objectName")))
                {
                    omp.write(object, _settings->value(omp.name()));
                }
            }
        }
        else
        {
            qDebug() << Q_FUNC_INFO
                     << "No midi interface found with portName:" << midiPort;
        }
        _settings->endGroup(); // group id
    }
    _settings->endGroup(); // "interface" group
    _settings->endGroup(); // "midi" group
}

void Minotor::loadLedMatrixSettings()
{
    _ledMatrix->openPortByName(_settings->value("serial/interface").toString());
}

void Minotor::saveSettings()
{
    _settings->setValue("renderer/size", _rendererSize);
    _settings->setValue("renderer/matrixSize", _matrixSize);
    _settings->setValue("renderer/panelSize", _panelSize);

    _settings->setValue("serial/interface", _ledMatrix->portName());

    _settings->beginGroup("midi");
    _settings->beginGroup("interface");
    // Remove all interfaces
    _settings->remove("");
    MidiInterfaces midiInterfaces = Minotor::minotor()->midi()->interfaces();
    int id = 0;
    for(int i=0; i<midiInterfaces.count(); i++)
    {
        MidiInterface *midiInterface = midiInterfaces.at(i);
        if(midiInterface->isUsed())
        {
            _settings->beginGroup(QString::number(id));
            QObject *object = static_cast<QObject*>(midiInterface);
            for(int j=0; j<object->metaObject()->propertyCount(); j++)
            {
                QMetaProperty omp = object->metaObject()->property(j);
                // Do not store "objectName"
                if(omp.name() != QString("objectName"))
                {
                    _settings->setValue(omp.name(), omp.read(object));
                }
            }
            _settings->endGroup();
            id++;
        }
    }
    _settings->endGroup(); // interface
    _settings->endGroup(); // midi
    _settings->sync();
}

void Minotor::save(MinoPersistentObject* object, QSettings* parser)
{
    if(object)
    {
        // Group name
        if(object->metaObject()->superClass() && (QString(object->metaObject()->superClass()->className()) == QString("MinoProgram")))
        {
            // HACK to transform hardcoded programs into a simple -and instantiable- MinoProgram
            parser->beginGroup("MinoProgram");
        }
        else
        {
            // Start a group using classname
            parser->beginGroup(object->metaObject()->className());
        }
        if(object->metaObject()->superClass())
        {
            if(QString(object->metaObject()->superClass()->className()) == QString("MinoProperty"))
            {
                if(object->objectName().isEmpty())
                {
                    qDebug() << Q_FUNC_INFO
                             << "object: " << object << "cant be saved properly (parent: " << object->parent() <<")";
                    Q_ASSERT(false);
                }
            } else if(QString(object->metaObject()->superClass()->className()) == QString("MidiControllableParameter"))
            {
                if(object->objectName().isEmpty())
                {
                    qDebug() << Q_FUNC_INFO
                             << "object: " << object << "cant be saved properly (parent: " << object->parent()->parent() <<")";
                    Q_ASSERT(false);
                }
            }
        }
        // Remove all entries in this group
        parser->remove("");
        qDebug() << QString(" ").repeated(2) << object;

        if(!object->objectName().isEmpty())
            parser->setValue("objectName", object->objectName());

        // Start an array of properties
        parser->beginWriteArray("properties");
        parser->remove("");

        for(int j=0; j<object->metaObject()->propertyCount(); j++)
        {
            parser->setArrayIndex(j);
            QMetaProperty omp = object->metaObject()->property(j);
            // Skip the already-handled objectName property
            if(QString(omp.name()) != QString("objectName"))
            {
                parser->setValue(omp.name(), omp.read(object));
                qDebug() << QString(" ").repeated(3)
                         << omp.typeName()
                         << omp.name()
                         << omp.read(object)
                         << omp.isStored();
            }
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

QObject *Minotor::findParentFor(const QString& className)
{
    QObject *parent = NULL;
    if(className == QString("MinoProgram"))
    {
        parent = _programBank;
    }
    else if (className == QString("MinoProgramBank"))
    {
        parent = this;
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "ERROR: no parent for class:" << className;
    }
    return parent;
}

void Minotor::loadObjects(QSettings *parser, QObject *parent)
{
    foreach(const QString& group, parser->childGroups())
    {
        qDebug() << Q_FUNC_INFO
                 << "group" << group;
        parser->beginGroup(group);
        loadObject(parser, group, parent);
        parser->endGroup();
    }
}

void Minotor::loadObject(QSettings *parser, const QString& className, QObject *parent)
{
    // Find the right parent
    if(!parent) {
        parent = findParentFor(className);
    }
    Q_ASSERT(parent);

    // Build or locate the object
    MinoPersistentObject *object = NULL;
    const QString objectName = parser->value("objectName").toString();
    if(objectName.isEmpty())
    {
        // no objectName, that means we create the object
        qDebug() << Q_FUNC_INFO
                 << "instantiate object from class:" << className;
        object = MinoPersistentObjectFactory::createObject(className.toAscii(), parent);
    }
    else
    {
        // objectName is filled, we will locate it from its parent
        qDebug() << Q_FUNC_INFO
                 << "find an object named:" << parser->value("objectName").toString();
        object = parent->findChild<MinoPersistentObject*>(objectName);
    }

    if(object)
    {
        // Restore properties value
        int size = parser->beginReadArray("properties");
        for(int i=0; i<size; ++i)
        {
            parser->setArrayIndex(i);
            foreach(const QString& key, parser->childKeys())
            {
                qDebug() << Q_FUNC_INFO
                         << QString("#%1 key:").arg(i) << key;
                int index = object->metaObject()->indexOfProperty(key.toAscii());
                if(index != -1)
                {
                    QMetaProperty omp = object->metaObject()->property(index) ;
                    omp.write(object, parser->value(key));
                    qDebug() << Q_FUNC_INFO
                             << "write property:" << key
                             << "with value:" << parser->value(key)
                             << "on object:" << object;
                }
            }
        }
        parser->endArray();

        // HACK: Group and Animations need to be explicitly added to their parent
        if(MinoAnimationGroup *group = qobject_cast<MinoAnimationGroup*>(object))
        {
            MinoProgram *program = qobject_cast<MinoProgram*>(parent);
            Q_ASSERT(program);
            program->addAnimationGroup(group);
        } else if (MinoAnimation *animation = qobject_cast<MinoAnimation*>(object))
        {
            MinoAnimationGroup *group = qobject_cast<MinoAnimationGroup*>(parent);
            Q_ASSERT(group);
            group->addAnimation(animation);
        }

        // Children
        size = parser->beginReadArray("children");
        for(int i=0; i<size; ++i)
        {
            parser->setArrayIndex(i);
            this->loadObjects(parser, object);
        }
        parser->endArray();
        // HACK: MinoProgram bank need to be reloaded
        if (MinoProgramBank *bank = qobject_cast<MinoProgramBank*>(object))
        {
            changeProgramBank(bank);
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "ERROR: no object to fill...";
    }
}

void Minotor::load(QSettings* parser)
{
    loadObjects(parser, NULL);
}

void Minotor::clearPrograms()
{
    MinoProgramBank *programBank = new MinoProgramBank(this);
    new MinoProgram(programBank);
    changeProgramBank(programBank);
}

void Minotor::changeProgramBank(MinoProgramBank *bank)
{
    _master->setProgram(NULL);
    delete _programBank;
    _programBank = bank;
    Q_ASSERT(bank);
    if(bank->programs().count())
    {
        // Use the first available program as master
        _master->setProgram(bank->programs().at(0));
    }
    emit programBankChanged(bank);
}

QPixmap* Minotor::graphicsItemToPixmap(QGraphicsItem *item)
{
    const qreal width = 24.0;
    const qreal height = 16.0;
    QPixmap *pixmap = new QPixmap(width, height);
    pixmap->fill(Qt::black);

    QPainter painter(pixmap);

    // Render a part of scene dedicated to screenshot (0,0)
    const QPointF screeningPos(0,0);
    QPointF fromScenePos = item->scenePos();
    QPointF fromPos = item->pos();
    item->setPos(item->mapFromScene(screeningPos));
    _scene.render(&painter, QRectF(0,0,width,height), QRectF(screeningPos, displayRect().size()), Qt::IgnoreAspectRatio);
    item->setPos(fromPos);
    Q_ASSERT(item->scenePos() == fromScenePos);

//    // Computes grids lines
//    QVector<QLine> gridLines;
//    qreal gridStepMin;

//    const QRect rect = displayRect();
//    gridLines.clear();
//    const qreal stepX =  width / rect.width();
//    const qreal stepY = height / rect.height();
//    for (int x = 1; x < rect.width(); x++)
//    {
//        int pos = x * stepX;
//        gridLines.append(QLine(pos,0,pos,height));
//    }
//    for (int y = 1; y < rect.height(); y++)
//    {
//        int pos = y * stepY;
//        gridLines.append(QLine(0,pos,width,pos));
//    }
//    gridStepMin = qMin(stepX, stepY);

//    QPen pen;
//    pen.setWidthF(gridStepMin*0.25);
//    pen.setColor(Qt::red);
//    painter.setPen(pen);

//    painter.drawLines(gridLines);

    return pixmap;
}

void Minotor::setRendererSize(const QSize &size)
{
     if (size.isValid())
     {
         if(_rendererSize != size)
         {
             _rendererSize = size;
             emit rendererSizeChanged();
         }
     }
}

void Minotor::setMatrixSize(const QSize &size)
{
     if (size.isValid())
     {
         if(_matrixSize != size)
         {
             _matrixSize = size;
         }
     }
}

void Minotor::setPanelSize(const QSize &size)
{
     if (size.isValid())
     {
         if(_panelSize != size)
         {
             _panelSize = size;
         }
     }
}

