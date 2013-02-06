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

    // Animations properties
    QScrollArea *sa = new QScrollArea(this);
    layout->addWidget(sa);


    _wContent = new QWidget(this);
    sa->setWidget(_wContent);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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

