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

#include "uimidiinterface.h"

#include <QLayout>
#include <QDebug>

#include "configdialog.h"
#include "midimapper.h"

UiMidiInterface::UiMidiInterface(MidiInterface *interface, QWidget *parent) :
    QWidget(parent)
{
    //Item 1
    QHBoxLayout *lMaster = new QHBoxLayout(this);
    this->setLayout(lMaster);

    //connection state
    QPushButton *_pbConnection = new QPushButton("",this);
    _pbConnection->setObjectName("tiny");
    _pbConnection->setCheckable(true);
    _pbConnection->setChecked(interface->isConnected());
    _pbConnection->setEnabled(false);
    _pbConnection->setFixedSize(9,9);
    connect(interface,SIGNAL(connected(bool)),_pbConnection,SLOT(setChecked(bool)));
    this->layout()->addWidget(_pbConnection);

    //Name
    _txtName = new QLabel(this);
    _txtName->setObjectName("interfacename");
    _txtName->setText(interface->portName() + QString("    ID: ") + QString::number(interface->id()));
    this->layout()->addWidget(_txtName);
    dynamic_cast<QHBoxLayout*>(this->layout())->addStretch(1);
    //Mapping : fake data
    _cbMapping = new QComboBox(this);
    _cbMapping->setToolTip("Select midi mapping to use with this interface");
    ConfigDialog* cd = qobject_cast<ConfigDialog*>(parent);
    cd->loadMidiMappingFiles(_cbMapping);
    int index;
    if((index = _cbMapping->findText(interface->mapping())) == -1)
    {
        qDebug() << Q_FUNC_INFO
                 << "mapping not found:" << interface->mapping();
        // Select "none"
        _cbMapping->setCurrentIndex(0);
    }
    else
    {
        _cbMapping->setCurrentIndex(index);
    }
    connect(_cbMapping,SIGNAL(currentIndexChanged(QString)),interface,SLOT(setMapping(QString)));

    this->layout()->addWidget(_cbMapping);
    //BeatSync
    _pbSync = new QPushButton("Sync",this);
    _pbSync->setToolTip("Receive sync data (Beatclock)");
    _pbSync->setCheckable(true);
    _pbSync->setChecked(interface->acceptClock());
    connect(_pbSync,SIGNAL(toggled(bool)),interface,SLOT(setAcceptClock(bool)));
    this->layout()->addWidget(_pbSync);
    //Notes
    _pbNotes = new QPushButton("Notes",this);
    _pbNotes->setToolTip("Receive midi notes");
    _pbNotes->setCheckable(true);
    _pbNotes->setChecked(interface->acceptNoteChange());
    connect(_pbNotes,SIGNAL(toggled(bool)),interface,SLOT(setAcceptNoteChange(bool)));
    this->layout()->addWidget(_pbNotes);
    //ControlChange
    _pbControlChange = new QPushButton("Ctrl Change",this);
    _pbControlChange->setToolTip("Receive midi control changes");
    _pbControlChange->setCheckable(true);
    _pbControlChange->setChecked(interface->acceptControlChange());
    connect(_pbControlChange,SIGNAL(toggled(bool)),interface,SLOT(setAcceptControlChange(bool)));
    this->layout()->addWidget(_pbControlChange);
    //ProgramChange
    _pbProgramChange = new QPushButton("Prg Change",this);
    _pbProgramChange->setToolTip("Receive midi program changes");
    _pbProgramChange->setCheckable(true);
    _pbProgramChange->setChecked(interface->acceptProgramChange());
    connect(_pbProgramChange,SIGNAL(toggled(bool)),interface,SLOT(setAcceptProgramChange(bool)));
    this->layout()->addWidget(_pbProgramChange);
}
