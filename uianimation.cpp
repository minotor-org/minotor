#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QMenu>
#include <QCheckBox>
#include <QToolButton>

#include <QDebug>

#include "uianimationproperty.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QGroupBox(parent)
{
    this->setMinimumWidth(80);
    this->setMaximumWidth(80);
    connect(animation, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    QWidget *wContent = new QWidget(this);
    wContent->setObjectName("content");
    lGroupBox->addWidget(wContent);
    QVBoxLayout *lContent = new QVBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);

    //Checkboxes
    _wEnable = new QWidget(wContent);
    QHBoxLayout *lEnable = new QHBoxLayout(_wEnable);
    _cbEnable = new QCheckBox(_wEnable);
    _cbEnable->setFocusPolicy(Qt::NoFocus);
    connect(_cbEnable, SIGNAL(toggled(bool)), animation, SLOT(setEnabled(bool)));
    QCheckBox *cbDelayedEnable = new QCheckBox(_wEnable);
    cbDelayedEnable->setFocusPolicy(Qt::NoFocus);
    connect(cbDelayedEnable, SIGNAL(toggled(bool)), animation, SLOT(setDelayedEnabled(bool)));
    lEnable->addStretch();
    lEnable->addWidget(_cbEnable);
    lEnable->addStretch();
    lEnable->addWidget(cbDelayedEnable);
    connect(_cbEnable, SIGNAL(toggled(bool)), cbDelayedEnable, SLOT(setChecked(bool)));
    lEnable->addStretch();
    lContent->addWidget(_wEnable);

    QWidget *wDescription = new QWidget(this);
    lContent->addWidget(wDescription);
    QHBoxLayout *lDescription = new QHBoxLayout(wDescription);
    lDescription->addStretch();
    MinoAnimationDescription desc = animation->description();
    _tAnimation = new QLabel(desc.name());
    _tAnimation->setObjectName("image");
    _tAnimation->setAlignment(Qt::AlignCenter);
    _tAnimation->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    _tAnimation->setMinimumSize(60,40);
    _tAnimation->setMaximumSize(60,40);

    lDescription->addWidget(_tAnimation);
    lDescription->addStretch();

    _wProperties = new QWidget(wContent);
    QVBoxLayout *lProperties = new QVBoxLayout(_wProperties);
    lContent->addWidget(_wProperties);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    foreach (MinoProperty *property, animation->properties())
    {
        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wProperties);
        connect(uiAnimationProperty, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        lProperties->addWidget(uiAnimationProperty);
    }
    lContent->addStretch();

    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));

}

void UiAnimation::enable(const bool on)
{
    _cbEnable->setChecked(on);
    if(on)
    {

        this->setStyleSheet("QGroupBox { background-color:#e75f00;}");
    }
    else
    {
        this->setStyleSheet("QGroupBox { background-color:#323232;}");
    }
}

void UiAnimation::setExpanded(bool expanded)
{
    _wEnable->setVisible(expanded);
    _wProperties->setVisible(expanded);

}
