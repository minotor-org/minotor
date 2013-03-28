#include "uimaster.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QDoubleSpinBox>
#include "uimastercontrol.h"
#include "minotor.h"

#include "uiprogramview.h"
#include "uimidiproperty.h"

#include "minotor.h"

UiMaster::UiMaster(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    connect(_master,SIGNAL(programChanged()),this,SLOT(updateProgram()));
    this->setMinimumHeight(336);
    this->setMaximumHeight(336);
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    this->setLayout(lMaster);
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
    lBackground->setContentsMargins(2,2,2,2);

    QWidget *wContainer = new QWidget(this);
    wContainer->setObjectName("panelcontent");
    lBackground->addWidget(wContainer);
    QVBoxLayout *lContainer = new QVBoxLayout(wContainer);
    lContainer->setSpacing(0);
    lContainer->setMargin(0);
    lContainer->setContentsMargins(0,0,0,0);

    //Title
    QLabel *tTitle = new QLabel(wContainer);
    tTitle->setObjectName("title");
    tTitle->setContentsMargins(10,5,0,0);
    lContainer->addWidget(tTitle);
    tTitle->setText("Master");


    //Content
    QWidget *wContent = new QWidget(wContainer);
    //wContent->setObjectName("panel");
    lContainer->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);

    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);
    //master left widget
    QWidget *leftArea = new QWidget(wContent);
    lContent->addWidget(leftArea);
    QVBoxLayout *lLeftArea = new QVBoxLayout(leftArea);
    // TODO add beat factor...
    (void)lLeftArea;
    leftArea->setMinimumSize(120,32);
    leftArea->setMaximumSize(120,320);

    //master control
    _uiMasterControl = new UiMasterControl(_master, wContent);
    connect(_master->program(),SIGNAL(animationMoved(int,int,int,int,int,int)),_uiMasterControl,SLOT(animationMoved(int,int,int,int,int,int)));
    connect(_master->program(),SIGNAL(animationGroupMoved(int,int,int)),_uiMasterControl,SLOT(animationGroupMoved(int,int,int)));
    lContent->addWidget(_uiMasterControl);

    //master view
    QWidget *wMasterView = new QWidget(wContent);
    lContent->addWidget(wMasterView);
    wMasterView->setMinimumWidth(264);
    wMasterView->setMaximumWidth(264);
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

    MinoPropertyReal *mpBrightness = dynamic_cast<MinoPropertyReal*>(_master->properties().at(0));
    if (mpBrightness)
    {
        UiMidiProperty *dBrightness = new UiMidiProperty(mpBrightness, wTools);
        lTools->addWidget(dBrightness);
    }
    lTools->addStretch();

    lMasterView->addStretch();
}

void UiMaster::updateProgram()
{
    disconnect(_uiMasterControl);
    _uiMasterMonitor->setProgram(_master->program());
    connect(_master->program(),SIGNAL(animationMoved(int,int,int,int,int,int)),_uiMasterControl,SLOT(animationMoved(int,int,int,int,int,int)));
    connect(_master->program(),SIGNAL(animationGroupMoved(int,int,int)),_uiMasterControl,SLOT(animationGroupMoved(int,int,int)));
}

