#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QStyle>
#include <QMenu>
#include <QCheckBox>
#include <QToolButton>

#include <QDebug>

#include "uianimationproperty.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QGroupBox(parent)
{
    this->setMinimumWidth(100);
    this->setMaximumWidth(100);
    connect(animation, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    lGroupBox->setSpacing(0);
    lGroupBox->setMargin(0);
    lGroupBox->setContentsMargins(1,1,1,1);
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
    lEnable->setSpacing(0);
    lEnable->setMargin(0);
    lEnable->setContentsMargins(0,5,0,0);
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
    lDescription->setSpacing(0);
    lDescription->setMargin(0);
    lDescription->setContentsMargins(0,3,0,0);
    lDescription->addStretch();
    MinoAnimationDescription desc = animation->description();
    _tAnimation = new QLabel(desc.name());
    if(!desc.pixmap().isNull())
        _tAnimation->setPixmap(desc.pixmap().scaled(60,40,Qt::KeepAspectRatio,Qt::SmoothTransformation));

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

    foreach (MinoPropertyList *group, animation->propertyGrouped())
    {
        QFrame *fSeparator = new QFrame(_wProperties);
        fSeparator->setObjectName("line");
        fSeparator->setFrameShape(QFrame::HLine);
        fSeparator->setFrameShadow(QFrame::Sunken);
        fSeparator->setLineWidth(1);

        QWidget *wPropGroup = new QWidget(_wProperties);
        lProperties->addWidget(wPropGroup);
        QHBoxLayout *lPropGroup = new QHBoxLayout(wPropGroup);
        lPropGroup->setSpacing(0);
        lPropGroup->setMargin(0);
        lPropGroup->setContentsMargins(0,0,0,0);

        for (int i=0;i<group->length();i++)
        {
            MinoProperty *property = group->at(i);
            UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wProperties);
            uiAnimationProperty->setObjectName("animationproperty");
            connect(uiAnimationProperty, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
            lPropGroup->addWidget(uiAnimationProperty);
        }

        lProperties->addWidget(fSeparator);
    }
    lProperties->addStretch();
    lContent->addStretch();
    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    this->enable(animation->enabled());
}

void UiAnimation::enable(const bool on)
{
    _cbEnable->setChecked(on);
    this->setProperty("active", on);

    this->style()->unpolish(this);
    this->style()->polish(this);
}

void UiAnimation::setExpanded(bool expanded)
{
    _wEnable->setVisible(expanded);
    _wProperties->setVisible(expanded);
}
