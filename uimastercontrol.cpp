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
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0,0,0,5);
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
    connect(_master,SIGNAL(programChanged()),this,SLOT(updateProgram()));
}

void UiMasterControl::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    uiAnimation->enable(animation->enabled());
    connect(uiAnimation, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));

    dynamic_cast<QBoxLayout*>(_wContent->layout())->insertWidget(_wContent->layout()->count()-1, uiAnimation);
}

void UiMasterControl::updateProgram()
{
    qDebug() << "updateProgram";
    foreach (UiAnimation *animation, this->findChildren<UiAnimation*>())
    {
        delete(animation);
    }
    foreach (MinoAnimation *animation, _master->program()->animations())
    {
        addAnimation(animation);

    }
}

UiMasterControl::~UiMasterControl()
{

}

