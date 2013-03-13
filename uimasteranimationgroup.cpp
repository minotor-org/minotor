#include "uimasteranimationgroup.h"

#include "uianimation.h"
#include "uianimationproperty.h"

#include "minoprogram.h"

#include <QStyle>
#include <QDebug>
#include <QPushButton>

UiMasterAnimationGroup::UiMasterAnimationGroup(MinoAnimationGroup *group, QWidget *parent) :
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

    _wContent = new QWidget(wBorder);
    lBorder->addWidget(_wContent);
    QVBoxLayout *lContent = new QVBoxLayout(_wContent);
    lContent->setSpacing(2);
    lContent->setMargin(0);
    lContent->setContentsMargins(1,1,1,1);

    QWidget *wTools = new QWidget(_wContent);
    lContent->addWidget(wTools);
    QHBoxLayout *lTools = new QHBoxLayout(wTools);
    lTools->setSpacing(0);
    lTools->setMargin(0);
    lTools->setContentsMargins(6,0,6,0);

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
    _pbEnable->setIconSize(QSize(20,20));
    _pbEnable->setMinimumSize(20,20);
    _pbEnable->setMaximumSize(20,20);
    _pbEnable->setCheckable(true);
    connect(_pbEnable, SIGNAL(toggled(bool)), _group, SLOT(setEnabled(bool)));
    lTools->addWidget(_pbEnable);
    connect(_pbEnable, SIGNAL(toggled(bool)), pbDelayedEnable, SLOT(setChecked(bool)));

    lTools->addStretch();

    QWidget *wRight = new QWidget(wTools);
    wRight->setMinimumSize(12,12);
    wRight->setMaximumSize(12,12);
    lTools->addWidget(wRight);

    QFrame *fSeparator = new QFrame(_wContent);
    fSeparator->setObjectName("groupLine");
    fSeparator->setFrameShape(QFrame::HLine);
    fSeparator->setFrameShadow(QFrame::Sunken);
    fSeparator->setLineWidth(1);
    lContent->addWidget(fSeparator);

    _wImportantParameters = new QWidget(_wContent);
    lContent->addWidget(_wImportantParameters);
    _lImportantParameters = new QVBoxLayout(_wImportantParameters);
    _lImportantParameters->setSpacing(4);
    _lImportantParameters->setMargin(0);
    _lImportantParameters->setContentsMargins(5,0,5,5);


    QFrame *fSeparator1 = new QFrame(_wContent);
    fSeparator1->setObjectName("groupLine");
    fSeparator1->setFrameShape(QFrame::HLine);
    fSeparator1->setFrameShadow(QFrame::Sunken);
    fSeparator1->setLineWidth(1);
    lContent->addWidget(fSeparator1);

    _wMidiParameters = new QWidget(_wContent);
    lContent->addWidget(_wMidiParameters);
    _lMidiParameters = new QVBoxLayout(_wMidiParameters);
    _lMidiParameters->setSpacing(4);
    _lMidiParameters->setMargin(0);
    _lMidiParameters->setContentsMargins(5,0,5,5);

    QFrame *fSeparator2 = new QFrame(_wContent);
    fSeparator2->setObjectName("groupLine");
    fSeparator2->setFrameShape(QFrame::HLine);
    fSeparator2->setFrameShadow(QFrame::Sunken);
    fSeparator2->setLineWidth(1);
    lContent->addWidget(fSeparator2);

    lContent->addStretch();

    // Connect UI with its group
    connect(_group, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    connect(_group, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    // Set policy
    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    // Update (delete,recreate) all components within a group
    this->updateGroup();

    // Update UI state of "enabled"
    this->enable(_group->enabled());
}

void UiMasterAnimationGroup::updateGroup()
{
    foreach (UiAnimationProperty *uiAnimationProperty, this->findChildren<UiAnimationProperty*>())
    {
        delete(uiAnimationProperty);
    }
    foreach (MinoAnimation *animation, _group->animations())
    {
        foreach (MinoPropertyList *list, animation->propertyGrouped())
        {
            foreach (MinoProperty *property, *list)
            {
                connect(property, SIGNAL(attributesChanged()), this, SLOT(updateGroup()));
                if(property->attributes().testFlag(MinoProperty::Important))
                {
                    UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wImportantParameters);
                    uiAnimationProperty->setObjectName("animationproperty");
                    _lImportantParameters->addWidget(uiAnimationProperty);
                } else if (property->attributes().testFlag(MinoProperty::MidiControled))
                {
                    UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wMidiParameters);
                    uiAnimationProperty->setObjectName("animationproperty");
                    _lMidiParameters->addWidget(uiAnimationProperty);
                }
            }
        }
    }
    _lImportantParameters->addStretch();
    _lMidiParameters->addStretch();
}

UiMasterAnimationGroup::~UiMasterAnimationGroup()
{
}

void UiMasterAnimationGroup::enable(const bool on)
{
    _pbEnable->setChecked(on);
    this->setProperty("active", on);
    this->style()->unpolish(this);
    this->style()->polish(this);
}

QSize UiMasterAnimationGroup::minimumSizeHint() const
{
    return QSize(150, 50);
}

QSize UiMasterAnimationGroup::sizeHint() const
{
    return QSize(150, 200);
}

