#include "uimastercontrol.h"
#include "uianimation.h"

#include "minoanimation.h"
#include "uiprogramview.h"

#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>

UiMasterControl::UiMasterControl(MinoMaster *master, QWidget *parent) :
    QWidget(parent),
    _master(master)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *wContent = new QWidget(this);
    layout->addWidget(wContent);
    wContent->setObjectName("panel");
    // Animations properties
    QScrollArea *sa = new QScrollArea(this);
    layout->addWidget(sa);


    _wContent = new QWidget(this);
    _wContent->setObjectName("scrollbackground");
    sa->setWidget(_wContent);
    sa->setFrameShadow(QFrame::Plain);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setWidgetResizable(true);

    QHBoxLayout *lContent = new QHBoxLayout(_wContent);
    lContent->addStretch();

    foreach (MinoAnimation *animation, _master->program()->animations())
    {
        addAnimation(animation);
    }

}

void UiMasterControl::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(_wContent->layout()->count()-1, uiAnimation);
}

UiMasterControl::~UiMasterControl()
{

}

