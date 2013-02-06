#include "uimaster.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QScrollArea>
#include <QDial>
#include <QLabel>
#include "uiprogrameditor.h"


#include "uiprogramview.h"

UiMaster::UiMaster(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    QVBoxLayout *lMaster = new QVBoxLayout(this);

    QWidget *wBackground = new QWidget(this);
    lMaster->addWidget(wBackground);
    wBackground->setStyleSheet("background-color:#3a3a3a;");

    // Master
    QVBoxLayout *lBackground = new QVBoxLayout(wBackground);

    //Title
    QLabel *tTitle = new QLabel(this);
    lBackground->addWidget(tTitle);
    tTitle->setStyleSheet("font:bold;font-size:14px;");
    tTitle->setText("Master");

    //Content
    QWidget *wContent = new QWidget(this);
    lBackground->addWidget(wContent);
    QHBoxLayout *lContent = new QHBoxLayout(wContent);
    UiProgramEditor *uiMasterEditor = new UiProgramEditor(_master->program(), wContent);
    lContent->addWidget(uiMasterEditor);

    //master view
    QWidget *wMasterView = new QWidget(wContent);
    lContent->addWidget(wMasterView);
    wMasterView->setMaximumWidth(240);
    QVBoxLayout *lMasterView = new QVBoxLayout(wMasterView);

    // Monitor
    QFrame *fMonitor = new QFrame(this);
    QVBoxLayout *lMonitor = new QVBoxLayout(fMonitor);
    fMonitor->setStyleSheet("background-color:#202020;");
    fMonitor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fMonitor->setMinimumSize(250, 170);
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
