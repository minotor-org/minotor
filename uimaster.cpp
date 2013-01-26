#include "uimaster.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QSlider>
#include <QDial>

#include "uichannelview.h"

UiMaster::UiMaster(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    // Master
    QVBoxLayout *lMaster = new QVBoxLayout(this);
    // Monitor
    QFrame *fMonitor = new QFrame(this);
    fMonitor->setFrameShape(QFrame::Box);

    fMonitor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    fMonitor->setMinimumSize(250, 170);
    lMaster->addWidget(fMonitor);

    QVBoxLayout *lMonitor = new QVBoxLayout(fMonitor);
    UiChannelView *uiMasterMonitor = new UiChannelView(_master, fMonitor);
    QSizePolicy policy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    policy.setHeightForWidth(true);
    uiMasterMonitor->setMinimumSize(240, 160);
    uiMasterMonitor->setSizePolicy(policy);

    QDial *dBrightness = new QDial(this);
    connect(dBrightness, SIGNAL(valueChanged(int)), this, SLOT(brightnessChanged(int)));
    dBrightness->setMinimum(0);
    dBrightness->setMaximum(127);
    lMaster->addWidget(dBrightness);

    lMonitor->addWidget(uiMasterMonitor);
}

void UiMaster::brightnessChanged(int value)
{
    _master->setBrightness((qreal)value/127);
}
