#include "uimaster.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QSlider>
#include <QDial>

#include "uimastermonitor.h"

UiMaster::UiMaster(Minotor *minotor, QWidget *parent) :
    QWidget(parent),
    _minotor(minotor)
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
    UiMasterMonitor *uiMasterMonitor = new UiMasterMonitor(minotor->ledMatrix(), fMonitor);
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
    lMaster->addStretch();

    QSlider *slider = new QSlider(Qt::Horizontal,this);
    slider->setMinimum(0);
    slider->setMaximum(127);
    lMaster->addWidget(slider);

    connect(slider, SIGNAL(valueChanged(int)), minotor->master(), SLOT(valueToViewPort(int)));
}

void UiMaster::brightnessChanged(int value)
{
    _minotor->master()->setBrightness((qreal)value/127);
}
