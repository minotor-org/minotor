#include "uianimationgroup.h"
#include "uianimation.h"

#include <QStyle>

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
    connect(_cbEnable, SIGNAL(toggled(bool)), group, SLOT(setEnabled(bool)));
    lEnable->addStretch();
    lEnable->addWidget(_cbEnable);

    QCheckBox *cbDelayedEnable = new QCheckBox(_wEnable);
    cbDelayedEnable->setFocusPolicy(Qt::NoFocus);
    cbDelayedEnable->setText("Activ  beat");
    connect(cbDelayedEnable, SIGNAL(toggled(bool)), group, SLOT(setDelayedEnabled(bool)));
    lEnable->addStretch();
    lEnable->addWidget(cbDelayedEnable);
    connect(_cbEnable, SIGNAL(toggled(bool)), cbDelayedEnable, SLOT(setChecked(bool)));

    QCheckBox *cbDelete = new QCheckBox(_wEnable);
    cbDelete->setFocusPolicy(Qt::NoFocus);
    cbDelete->setText("Delete");
    connect(cbDelete, SIGNAL(toggled(bool)), group, SLOT(deleteLater()));
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
    enable(group->enabled());
    connect(group, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    connect(group, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}

void UiAnimationGroup::addAnimation(MinoAnimation *animation)
{
    UiAnimation *uiAnimation = new UiAnimation(animation, _wContent);
    uiAnimation->setExpanded(_expanded);

    _lContent->insertWidget(_lContent->count()-1, uiAnimation);
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
    //qDebug() <<
}
