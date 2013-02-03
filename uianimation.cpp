#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QMenu>
#include <QLabel>

#include <QToolButton>

#include <QDebug>

#include "uianimationproperty.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QFrame(parent)
{
    setFrameShape(QFrame::Panel);
    QHBoxLayout *lAnimation = new QHBoxLayout(this);

    MinoAnimationDescription desc = animation->description();
    QLabel *tAnimation = new QLabel(desc.name());
    tAnimation->setAlignment(Qt::AlignCenter);
    tAnimation->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    tAnimation->setMinimumWidth(100);
    tAnimation->setStyleSheet("background-color:yellow;");
    lAnimation->addWidget(tAnimation);

    QToolButton *tbDelete = new QToolButton(this);
    connect(tbDelete, SIGNAL(clicked()), animation, SLOT(deleteLater()));
    lAnimation->addWidget(tbDelete);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    foreach (MinoAnimationProperty *property, animation->properties())
    {
        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, this);
        connect(uiAnimationProperty, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        lAnimation->addWidget(uiAnimationProperty);
    }
    lAnimation->addStretch();

    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}
