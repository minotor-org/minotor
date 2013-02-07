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
#include "minotor.h"


#include "uiprogramview.h"

UiMaster::UiMaster(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    connect(_master,SIGNAL(programChanged()),this,SLOT(updateProgram()));
    this->setMaximumHeight(335);
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    lMaster->setSpacing(0);
    lMaster->setMargin(0);
    lMaster->setContentsMargins(0,0,0,0);
    QWidget *wBackground = new QWidget(this);
    wBackground->setObjectName("panel");
    lMaster->addWidget(wBackground);

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);
    lBackground->setSpacing(0);
    lBackground->setMargin(0);
    lBackground->setContentsMargins(0,0,0,0);
    //Title
    QLabel *tTitle = new QLabel(this);
    tTitle->setObjectName("title");
    tTitle->setContentsMargins(5,5,0,0);
    lBackground->addWidget(tTitle);
    tTitle->setText("Master");

    //Content
    QWidget *wContent = new QWidget(this);
    //wContent->setObjectName("panel");
    lBackground->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);

    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);
    //master left widget
    QWidget *leftArea = new QWidget(wContent);
    lContent->addWidget(leftArea);
    QVBoxLayout *lLeftArea = new QVBoxLayout(leftArea);
    leftArea->setMinimumSize(72,20);
    leftArea->setMaximumSize(72,20);
    //master control
    UiMasterControl *uiMasterControl = new UiMasterControl(_master, wContent);
    lContent->addWidget(uiMasterControl);

    //master view
    QWidget *wMasterView = new QWidget(wContent);
    lContent->addWidget(wMasterView);
    wMasterView->setMaximumWidth(250);
    QVBoxLayout *lMasterView = new QVBoxLayout(wMasterView);

    // Monitor
    QFrame *fMonitor = new QFrame(this);
    QVBoxLayout *lMonitor = new QVBoxLayout(fMonitor);
    fMonitor->setObjectName("view");
    fMonitor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fMonitor->setMinimumSize(240, 160);
    lMasterView->addWidget(fMonitor);
    _uiMasterMonitor = new UiProgramView(_master->program(), fMonitor);
    _uiMasterMonitor->setObjectName("masterMonitor");
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    lMonitor->addWidget(_uiMasterMonitor);

    //Monitor tools
    QWidget *wTools = new QWidget(wMasterView);
    lMasterView->addWidget(wTools);
    QHBoxLayout *lTools = new QHBoxLayout(wTools);


    QDial *dBrightness = new QDial(wTools);
    dBrightness->setMaximumSize(25,25);
    connect(dBrightness, SIGNAL(valueChanged(int)), this, SLOT(brightnessChanged(int)));
    dBrightness->setMinimum(0);
    dBrightness->setMaximum(127);
    lTools->addWidget(dBrightness);
    QLabel *tBrightness = new QLabel(wTools);
    lTools->addWidget(tBrightness);
    tBrightness->setText("Brightness");

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
