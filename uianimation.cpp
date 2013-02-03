#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QMenu>
#include <QLabel>

#include <QToolButton>

#include <QDebug>

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
        QWidget *wProperty = new QWidget(this);
        QVBoxLayout *lProperty = new QVBoxLayout(wProperty);
        UiDial *d = new UiDial(property, wProperty);
        d->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        connect(d, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
        lProperty->addWidget(d);
        QLabel *t = new QLabel(QString(property->objectName()), wProperty);
        t->setAlignment(Qt::AlignCenter);
        lProperty->addWidget(t);
        lAnimation->addWidget(wProperty);
    }
    lAnimation->addStretch();

    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}
