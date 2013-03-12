#include "uianimationgroup.h"
#include "uianimation.h"
#include "minoprogram.h"

#include <QStyle>
#include <QDebug>

UiAnimationGroup::UiAnimationGroup(MinoAnimationGroup *group, QWidget *parent) :
    QGroupBox(parent),
    _group(group)
{
    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    lGroupBox->setSpacing(0);
    lGroupBox->setMargin(0);
    lGroupBox->setContentsMargins(1,1,1,1);

    QWidget *wBorder = new QWidget(this);
    wBorder->setObjectName("group");
    lGroupBox->addWidget(wBorder);
    QVBoxLayout *lBorder = new QVBoxLayout(wBorder);
    lBorder->setSpacing(0);
    lBorder->setMargin(0);
    lBorder->setContentsMargins(1,1,1,1);


    //Checkboxes
    _wEnable = new QWidget(wBorder);
    _wEnable->setObjectName("content");
    lBorder->addWidget(_wEnable);
    QVBoxLayout *lEnable = new QVBoxLayout(_wEnable);
    lEnable->setSpacing(5);
    lEnable->setMargin(0);
    lEnable->setContentsMargins(5,5,0,2);
    _cbEnable = new QCheckBox(_wEnable);
    _cbEnable->setFocusPolicy(Qt::NoFocus);
    _cbEnable->setText("Activ");
    connect(_cbEnable, SIGNAL(toggled(bool)), _group, SLOT(setEnabled(bool)));
    lEnable->addStretch();
    lEnable->addWidget(_cbEnable);

    QCheckBox *cbDelayedEnable = new QCheckBox(_wEnable);
    cbDelayedEnable->setFocusPolicy(Qt::NoFocus);
    cbDelayedEnable->setText("Activ  beat");
    connect(cbDelayedEnable, SIGNAL(toggled(bool)), _group, SLOT(setDelayedEnabled(bool)));
    lEnable->addStretch();
    lEnable->addWidget(cbDelayedEnable);
    connect(_cbEnable, SIGNAL(toggled(bool)), cbDelayedEnable, SLOT(setChecked(bool)));

    QCheckBox *cbDelete = new QCheckBox(_wEnable);
    cbDelete->setFocusPolicy(Qt::NoFocus);
    cbDelete->setText("Delete");
    connect(cbDelete, SIGNAL(toggled(bool)), _group, SLOT(deleteLater()));
    lEnable->addWidget(cbDelete);

    lEnable->addStretch();


    _wContent = new QWidget(wBorder);
    _wContent->setObjectName("content");
    lBorder->addWidget(_wContent);
    _lContent = new QHBoxLayout(_wContent);
    _lContent->setSpacing(5);
    _lContent->setMargin(0);
    _lContent->setContentsMargins(5,0,5,5);

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
    qDebug() << "~UiAnimationGroup>"
             << "_lContent->count():" << _lContent->count()
             << "uiAnimations count:" << _wContent->findChildren<UiAnimation*>().count();

}

void UiAnimationGroup::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    connect(uiAnimation, SIGNAL(animationMoved(int,int)), this, SLOT(_moveAnimation(int,int)));
    uiAnimation->setExpanded(_expanded);
    _lContent->insertWidget(_lContent->count(), uiAnimation);
}

void UiAnimationGroup::insertAnimation(UiAnimation *animation, int destId)
{
    _lContent->insertWidget(destId, animation);
    animation->setParent(_wContent);
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
        if(groupId == _group->program()->animationGroups().indexOf(_group))
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
    _cbEnable->setChecked(on);
    this->setProperty("active", on);
    this->style()->unpolish(this);
    this->style()->polish(this);
}

UiAnimation* UiAnimationGroup::takeAt(int index)
{
    QLayoutItem *li = _lContent->takeAt(index);
    if(li->widget())
    {
        return dynamic_cast<UiAnimation*>(li->widget());
    }
    else
    {
        return NULL;
    }
}
