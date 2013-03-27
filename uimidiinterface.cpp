#include "uimidiinterface.h"
#include <QLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

UiMidiInterface::UiMidiInterface(QWidget *parent) :
    QWidget(parent)
{
    //Item 1
    this->setLayout(new QHBoxLayout());

    //Name
    QLabel *txtName = new QLabel(this);
    txtName->setObjectName("interfacename");
    txtName->setText("Korg nanoKontrol");
    this->layout()->addWidget(txtName);
    dynamic_cast<QHBoxLayout*>(this->layout())->addStretch(1);
    //Mapping
    QComboBox *cbMapping = new QComboBox(this);
    cbMapping->addItem("no mapping");
    cbMapping->addItem("Akai LPD8");
    cbMapping->addItem("Korg nanoKontrol");
    this->layout()->addWidget(cbMapping);
    //BeatSync
    QPushButton *pbSync = new QPushButton("Sync",this);
    pbSync->setCheckable(true);
    pbSync->setChecked(false);
    this->layout()->addWidget(pbSync);
    //Notes
    QPushButton *pbNotes = new QPushButton("Notes",this);
    pbNotes->setCheckable(true);
    pbNotes->setChecked(false);
    this->layout()->addWidget(pbNotes);
    //ControlChange
    QPushButton *pbControlChange = new QPushButton("Ctrl Change",this);
    pbControlChange->setCheckable(true);
    pbControlChange->setChecked(true);
    this->layout()->addWidget(pbControlChange);
    //ProgramChange
    QPushButton *pbProgramChange = new QPushButton("Prg Change",this);
    pbProgramChange->setCheckable(true);
    pbProgramChange->setChecked(false);
    this->layout()->addWidget(pbProgramChange);

}
