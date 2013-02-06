#include "uimaster.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QDial>
#include <QLabel>
#include "uimastercontrol.h"


#include "uiprogramview.h"

UiMaster::UiMaster(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    this->setMaximumHeight(335);
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    QWidget *wBackground = new QWidget(this);
    wBackground->setObjectName("panel");
    lMaster->addWidget(wBackground);

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);

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
    wMasterView->setMaximumWidth(244);
    QVBoxLayout *lMasterView = new QVBoxLayout(wMasterView);

    // Monitor
    QFrame *fMonitor = new QFrame(this);
    QVBoxLayout *lMonitor = new QVBoxLayout(fMonitor);
    fMonitor->setObjectName("view");
    fMonitor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fMonitor->setMinimumSize(240, 160);
    lMasterView->addWidget(fMonitor);
    UiProgramView *uiMasterMonitor = new UiProgramView(_master->program(), fMonitor);
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    lMonitor->addWidget(uiMasterMonitor);

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

    lTools->addStretch();
    lMasterView->addStretch();


}

void UiMaster::brightnessChanged(int value)
{
    _master->setBrightness((qreal)value/127);
}
