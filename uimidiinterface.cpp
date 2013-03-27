#include "uimidiinterface.h"
#include <QLayout>

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
    _txtName->setText(interface->portName());
    this->layout()->addWidget(_txtName);
    dynamic_cast<QHBoxLayout*>(this->layout())->addStretch(1);
    //Mapping : fake data
    _cbMapping = new QComboBox(this);
    _cbMapping->addItem("no mapping");
    _cbMapping->addItem("Akai LPD8");
    _cbMapping->addItem("Korg nanoKontrol");
    this->layout()->addWidget(_cbMapping);
    //BeatSync
    _pbSync = new QPushButton("Sync",this);
    _pbSync->setCheckable(true);
    _pbSync->setChecked(interface->acceptClock());
    connect(_pbSync,SIGNAL(toggled(bool)),interface,SLOT(setAcceptClock(bool)));
    this->layout()->addWidget(_pbSync);
    //Notes
    _pbNotes = new QPushButton("Notes",this);
    _pbNotes->setCheckable(true);
    _pbNotes->setChecked(interface->acceptNoteChange());
    connect(_pbNotes,SIGNAL(toggled(bool)),interface,SLOT(setAcceptNoteChange(bool)));
    this->layout()->addWidget(_pbNotes);
    //ControlChange
    _pbControlChange = new QPushButton("Ctrl Change",this);
    _pbControlChange->setCheckable(true);
    _pbControlChange->setChecked(interface->acceptControlChange());
    connect(_pbControlChange,SIGNAL(toggled(bool)),interface,SLOT(setAcceptControlChange(bool)));
    this->layout()->addWidget(_pbControlChange);
    //ProgramChange
    _pbProgramChange = new QPushButton("Prg Change",this);
    _pbProgramChange->setCheckable(true);
    _pbProgramChange->setChecked(interface->acceptProgramChange());
    connect(_pbProgramChange,SIGNAL(toggled(bool)),interface,SLOT(setAcceptProgramChange(bool)));
    this->layout()->addWidget(_pbProgramChange);

}
