#include "uimaster.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QDial>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QDoubleSpinBox>
#include "uimastercontrol.h"


#include "uiprogramview.h"

UiMaster::UiMaster(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    connect(_master,SIGNAL(programChanged()),this,SLOT(updateProgram()));
    this->setMaximumHeight(335);
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    QWidget *wBackground = new QWidget(this);
    wBackground->setObjectName("panel");
    lMaster->addWidget(wBackground);

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);
    lBackground->setSpacing(0);
    lBackground->setMargin(0);
    lBackground->setContentsMargins(5,5,0,0);
    //Title
    QLabel *tTitle = new QLabel(this);
    tTitle->setObjectName("title");
    lBackground->addWidget(tTitle);
    tTitle->setText("Master");

    //Content
    QWidget *wContent = new QWidget(this);
    wContent->setObjectName("panel");
    lBackground->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);
    //master left widget
    QWidget *leftArea = new QWidget(wContent);
    lContent->addWidget(leftArea);
    QVBoxLayout *lLeftArea = new QVBoxLayout(leftArea);
    leftArea->setMinimumSize(66,20);
    leftArea->setMaximumSize(66,20);
    //master control
    UiMasterControl *uiMasterControl = new UiMasterControl(_master, wContent);
    lContent->addWidget(uiMasterControl);

    //master view
    QWidget *wMasterView = new QWidget(wContent);
    lContent->addWidget(wMasterView);
    wMasterView->setMaximumWidth(245);
    QVBoxLayout *lMasterView = new QVBoxLayout(wMasterView);

    // Monitor
    QFrame *fMonitor = new QFrame(this);
    QVBoxLayout *lMonitor = new QVBoxLayout(fMonitor);
    fMonitor->setObjectName("view");
    fMonitor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fMonitor->setMinimumSize(240, 160);
    lMasterView->addWidget(fMonitor);
    _uiMasterMonitor = new UiProgramView(_master->program(), fMonitor);
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    lMonitor->addWidget(_uiMasterMonitor);

    //Monitor tools
    QWidget *wTools = new QWidget(wMasterView);
    lMasterView->addWidget(wTools);
    QVBoxLayout *lTools = new QVBoxLayout(wTools);

    QWidget *wBPMTools = new QWidget(wTools);
    lTools->addWidget(wBPMTools);
    QHBoxLayout *lBPMTools = new QHBoxLayout(wBPMTools);

    QDial *dBrightness = new QDial(wTools);
    dBrightness->setMaximumSize(25,25);
    connect(dBrightness, SIGNAL(valueChanged(int)), this, SLOT(brightnessChanged(int)));
    dBrightness->setMinimum(0);
    dBrightness->setMaximum(127);
    lBPMTools->addWidget(dBrightness);

    // Sync
    QPushButton *pbTap = new QPushButton(wTools);
    lBPMTools->addWidget(pbTap);
    pbTap->setText("Tap");
    connect(pbTap,SIGNAL(clicked(bool)),_master->program()->minotor()->clockSource(),SLOT(uiTapOn()));
    QDoubleSpinBox *_sbBPM = new QDoubleSpinBox(wBPMTools);
    _sbBPM->setMinimum(20);
    _sbBPM->setMaximum(300);
    _sbBPM->setSingleStep(0.1);
    _sbBPM->setDecimals(1);
    lBPMTools->addWidget(_sbBPM);
    _sbBPM->setValue(_master->program()->minotor()->clockSource()->bpm());
    connect(_master->program()->minotor()->clockSource(), SIGNAL(bpmChanged(double)), _sbBPM, SLOT(setValue(double)));
    connect(_sbBPM,SIGNAL(valueChanged(double)),_master->program()->minotor()->clockSource(),SLOT(setBPM(double)));
    /*
    QLCDNumber *lcdBpm = new QLCDNumber();
    lBPMTools->addWidget(lcdBpm);
    lcdBpm->setFrameShape(QFrame::Panel);
    lcdBpm->setFrameShadow(QFrame::Plain);
    lcdBpm->setSegmentStyle(QLCDNumber::Flat);
    lcdBpm->setMaximumHeight(17);
    lcdBpm->display(_master->program()->minotor()->clockSource()->bpm());
    connect(_master->program()->minotor()->clockSource(), SIGNAL(bpmChanged(double)), lcdBpm, SLOT(display(double)));
*/
    //Transport
    QWidget *wTransport = new QWidget(wTools);
    lTools->addWidget(wTransport);
    QHBoxLayout *lTransport = new QHBoxLayout(wTransport);

    QPushButton *pbStart = new QPushButton(wTools);
    lTransport->addWidget(pbStart);
    pbStart->setText("Start");
    connect(pbStart,SIGNAL(clicked(bool)),_master->program()->minotor()->clockSource(),SLOT(uiStart()));

    QPushButton *pbStop = new QPushButton(wTools);
    lTransport->addWidget(pbStop);
    pbStop->setText("Stop");
    connect(pbStop,SIGNAL(clicked(bool)),_master->program()->minotor()->clockSource(),SLOT(uiStop()));

    QPushButton *pbSync = new QPushButton(wTools);
    lTransport->addWidget(pbSync);
    pbSync->setText("Sync");
    connect(pbSync,SIGNAL(clicked(bool)),_master->program()->minotor()->clockSource(),SLOT(uiSync()));


    // MIDI toolbar
    QWidget *wMidi = new QWidget(wTools);
    lTools->addWidget(wMidi);
    QHBoxLayout *lMidi = new QHBoxLayout(wMidi);
    QPushButton *pbMidiLearn = new QPushButton(wMidi);
    connect(pbMidiLearn,SIGNAL(toggled(bool)),this,SIGNAL(midiLearnToggled(bool)));
    pbMidiLearn->setText("MIDI learn");
    pbMidiLearn->setCheckable(true);
    lMidi->addWidget(pbMidiLearn);


    lTools->addStretch();
    //
    lMasterView->addStretch();


}

void UiMaster::brightnessChanged(int value)
{
    _master->setBrightness((qreal)value/127);
}

void UiMaster::updateProgram()
{
    _uiMasterMonitor->setProgram(_master->program());
}
