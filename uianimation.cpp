#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QMenu>

#include <QToolButton>

#include <QDebug>

#include "uianimationproperty.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QFrame(parent)
{
    setFrameShape(QFrame::Panel);
    QHBoxLayout *lAnimation = new QHBoxLayout(this);

    MinoAnimationDescription desc = animation->description();
    _tAnimation = new QLabel(desc.name());
    _tAnimation->setAlignment(Qt::AlignCenter);
    _tAnimation->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    _tAnimation->setMinimumWidth(100);
    _tAnimation->setStyleSheet("background-color:yellow;");
    lAnimation->addWidget(_tAnimation);

    QToolButton *tbDelete = new QToolButton(this);
    connect(tbDelete, SIGNAL(clicked()), animation, SLOT(deleteLater()));
    lAnimation->addWidget(tbDelete);

    QToolButton *tbEnable = new QToolButton(this);
    tbEnable->setCheckable(true);
    tbEnable->setChecked(animation->enabled());
    connect(tbEnable, SIGNAL(toggled(bool)), animation, SLOT(setEnabled(bool)));
    lAnimation->addWidget(tbEnable);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    foreach (MinoProperty *property, animation->properties())
    {
        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, this);
        connect(uiAnimationProperty, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        lAnimation->addWidget(uiAnimationProperty);
    }
    lAnimation->addStretch();

    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));
    connect(animation, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
}

void UiAnimation::enable(const bool on)
{
    if(on)
    {
        _tAnimation->setStyleSheet("background-color:green;");
    }
    else
    {
        _tAnimation->setStyleSheet("background-color:red;");
    }
}
