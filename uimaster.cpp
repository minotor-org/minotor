#include "uimaster.h"
#include <QVBoxLayout>
#include <QFrame>
#include "uimastermonitor.h"
#include "minotor.h"
#include <QSlider>

UiMaster::UiMaster(Minotor *minotor, QWidget *parent) :
    QWidget(parent)

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

    lMonitor->addWidget(uiMasterMonitor);
    lMaster->addStretch();

    QSlider *slider = new QSlider(Qt::Horizontal,this);
    slider->setMinimum(0);
    slider->setMaximum(127);
    lMaster->addWidget(slider);

    connect(slider,SIGNAL(valueChanged(int)),minotor->master(),SLOT(valueToViewPort(int)));

}
