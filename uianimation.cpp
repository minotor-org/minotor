#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QMenu>

#include <QToolButton>

#include <QDebug>

#include "uianimationproperty.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QGroupBox(parent)
{
    this->setCheckable(true);
    this->setChecked(animation->enabled());
    connect(this, SIGNAL(toggled(bool)), animation, SLOT(setEnabled(bool)));
    connect(animation, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    QWidget *wContent = new QWidget(this);
    wContent->setObjectName("content");
    lGroupBox->addWidget(wContent);
    QVBoxLayout *lContent = new QVBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);

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
/*
    QToolButton *tbEnable = new QToolButton(this);
    tbEnable->setCheckable(true);
    tbEnable->setChecked(animation->enabled());
    connect(tbEnable, SIGNAL(toggled(bool)), animation, SLOT(setEnabled(bool)));
    lAnimation->addWidget(tbEnable);
*/
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    foreach (MinoProperty *property, animation->properties())
    {
        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, this);
        connect(uiAnimationProperty, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        lContent->addWidget(uiAnimationProperty);
    }
    lContent->addStretch();

    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}

void UiAnimation::enable(const bool on)
{
    this->setChecked(on);
    if(on)
    {

        this->setStyleSheet("QGroupBox { background-color:#e75f00;}");
    }
    else
    {
        this->setStyleSheet("QGroupBox { background-color:#323232;}");
    }
}
