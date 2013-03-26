#include "uianimationgroup.h"
#include "uianimation.h"
#include "minoprogram.h"

#include <QStyle>
#include <QDebug>
#include <QPushButton>

UiAnimationGroup::UiAnimationGroup(MinoAnimationGroup *group, QWidget *parent) :
    QGroupBox(parent),
    _group(group)
{
    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    lGroupBox->setSpacing(0);
    lGroupBox->setMargin(0);
    lGroupBox->setContentsMargins(2,2,2,2);

    QWidget *wBorder = new QWidget(this);
    wBorder->setObjectName("group");
    lGroupBox->addWidget(wBorder);
    QVBoxLayout *lBorder = new QVBoxLayout(wBorder);
    lBorder->setSpacing(0);
    lBorder->setMargin(0);
    lBorder->setContentsMargins(1,1,1,1);

    _wContent = new QWidget(wBorder);
    lBorder->addWidget(_wContent);
    QVBoxLayout *lContent = new QVBoxLayout(_wContent);
    lContent->setSpacing(2);
    lContent->setMargin(0);
    lContent->setContentsMargins(1,1,1,1);

    QWidget *wTools = new QWidget(_wContent);
    lContent->addWidget(wTools);
    wTools->setObjectName("titlebar");
    QHBoxLayout *lTools = new QHBoxLayout(wTools);
    lTools->setSpacing(0);
    lTools->setMargin(0);
    lTools->setContentsMargins(6,4,6,4);

    //Delayed button
    QPushButton *pbDelayedEnable = new QPushButton(wTools);
    pbDelayedEnable->setFocusPolicy(Qt::NoFocus);
    pbDelayedEnable->setIcon(QIcon(":/pictos/delayed.png"));
    pbDelayedEnable->setIconSize(QSize(8,8));
    pbDelayedEnable->setMinimumSize(12,12);
    pbDelayedEnable->setMaximumSize(12,12);
    pbDelayedEnable->setCheckable(true);
    connect(pbDelayedEnable, SIGNAL(toggled(bool)), _group, SLOT(setDelayedEnabled(bool)));
    lTools->addWidget(pbDelayedEnable);

    lTools->addStretch();

    //Enable button
    _pbEnable = new QPushButton(wTools);
    _pbEnable->setFocusPolicy(Qt::NoFocus);
    _pbEnable->setIcon(QIcon(":/pictos/power.png"));
    _pbEnable->setIconSize(QSize(18,18));
    _pbEnable->setMinimumSize(18,18);
    _pbEnable->setMaximumSize(18,18);
    _pbEnable->setCheckable(true);
    connect(_pbEnable, SIGNAL(toggled(bool)), _group, SLOT(setEnabled(bool)));
    lTools->addWidget(_pbEnable);

    lTools->addStretch();

    //Delete button
    QPushButton *pbDelete = new QPushButton(wTools);
    pbDelete->setFocusPolicy(Qt::NoFocus);
    pbDelete->setIcon(QIcon(":/pictos/close.png"));
    pbDelete->setIconSize(QSize(8,8));
    pbDelete->setMinimumSize(12,12);
    pbDelete->setMaximumSize(12,12);
    pbDelete->setCheckable(true);
    connect(pbDelete, SIGNAL(toggled(bool)), _group, SLOT(deleteLater()));
    lTools->addWidget(pbDelete);

    QFrame *fSeparator = new QFrame(_wContent);
    fSeparator->setObjectName("groupline");
    fSeparator->setFrameShape(QFrame::HLine);
    fSeparator->setFrameShadow(QFrame::Sunken);
    fSeparator->setLineWidth(1);
    lContent->addWidget(fSeparator);

     _wAnimations = new QWidget(_wContent);
    lContent->addWidget(_wAnimations);
    _lAnimations = new QHBoxLayout(_wAnimations);
    _lAnimations->setSpacing(5);
    _lAnimations->setMargin(0);
    _lAnimations->setContentsMargins(0,0,0,0);

    foreach (MinoAnimation *animation, _group->animations())
    {
        addAnimation(animation);
    }
    this->enable(_group->enabled());
    this->setExpanded(true);
    connect(_group, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    connect(_group, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}

UiAnimationGroup::~UiAnimationGroup()
{
}

void UiAnimationGroup::addAnimation(MinoAnimation *animation, int index)
{
    if (index == -1)
        index = _lAnimations->count();
    UiAnimation *uiAnimation = new UiAnimation(animation, _wAnimations);
    connect(uiAnimation, SIGNAL(animationMoved(int,int)), this, SLOT(_moveAnimation(int,int)));
    uiAnimation->setExpanded(_expanded);
    _lAnimations->insertWidget(index, uiAnimation);
}

void UiAnimationGroup::insertAnimation(UiAnimation *animation, int destId)
{
    _lAnimations->insertWidget(destId, animation);
    animation->setParent(_wAnimations);
}

void UiAnimationGroup::moveAnimation(int srcId, int destId)
{
    UiAnimation *animation = this->takeAt(srcId);
    if(animation)
    {
        insertAnimation(animation, destId);
    }
}

void UiAnimationGroup::_moveAnimation(int programId, int groupId)
{
    qDebug() << "UiAnimationGroup>"
             << "sender:" << sender();
    if(programId == _group->program()->id())
    {
        // Same program between src and dest
        if(groupId == _group->id())
        {
            // and same groupid
        }
        else
        {
            qDebug() << "UiAnimationGroup> group is different between src and dest";
        }
    }
}

void UiAnimationGroup::setExpanded(bool on)
{
    _expanded = on;
    foreach (UiAnimation *animation,this->findChildren<UiAnimation*>())
    {
        animation->setExpanded(on);
    }
}

void UiAnimationGroup::enable(const bool on)
{
    _pbEnable->setChecked(on);
    this->setProperty("active", on);
    this->style()->unpolish(this);
    this->style()->polish(this);
}

UiAnimation* UiAnimationGroup::takeAt(int index)
{
    QLayoutItem *li = _lAnimations->takeAt(index);
    if(li->widget())
    {
        return dynamic_cast<UiAnimation*>(li->widget());
    }
    else
    {
        return NULL;
    }
}
