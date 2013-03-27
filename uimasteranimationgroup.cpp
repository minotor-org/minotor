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
    enum Qt::Key notesKeys[6] = { Qt::Key_W, Qt::Key_X, Qt::Key_C, Qt::Key_V, Qt::Key_B, Qt::Key_N };

    const int groupId = _group->id();
    if((groupId>=0)&&(groupId<6))
    {
        _pbEnable->setShortcut(notesKeys[groupId]);
    }

    _pbEnable->setFocusPolicy(Qt::NoFocus);
    _pbEnable->setIcon(QIcon(":/pictos/power.png"));
    _pbEnable->setIconSize(QSize(18,18));
    _pbEnable->setMinimumSize(18,18);
    _pbEnable->setMaximumSize(18,18);
    _pbEnable->setCheckable(true);
    connect(_pbEnable, SIGNAL(toggled(bool)), _group, SLOT(setEnabled(bool)));
    lTools->addWidget(_pbEnable);

    lTools->addStretch();

    QWidget *wRight = new QWidget(wTools);
    wRight->setMinimumSize(12,12);
    wRight->setMaximumSize(12,12);
    lTools->addWidget(wRight);

    QFrame *fSeparator = new QFrame(_wContent);
    fSeparator->setObjectName("groupline");
    fSeparator->setFrameShape(QFrame::HLine);
    fSeparator->setFrameShadow(QFrame::Sunken);
    fSeparator->setLineWidth(1);
    lContent->addWidget(fSeparator);

    QGroupBox *gbParameters = new QGroupBox(_wContent);
    lContent->addWidget(gbParameters);
    gbParameters->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    QVBoxLayout *lParameters = new QVBoxLayout(gbParameters);
    lParameters->setSpacing(0);
    lParameters->setMargin(0);
    lParameters->setContentsMargins(1,1,1,1);
    _wGroupParameters  = new QWidget(gbParameters);
    lParameters->addWidget(_wGroupParameters);
    _wGroupParameters->setObjectName("content");
    QVBoxLayout *lGroupParameters = new QVBoxLayout(_wGroupParameters);
    lGroupParameters->setSpacing(2);
    lGroupParameters->setMargin(0);
    lGroupParameters->setContentsMargins(1,1,1,1);

    _wImportantParameters = new QWidget(_wContent);
    lGroupParameters->addWidget(_wImportantParameters);
    _lImportantParameters = new QVBoxLayout(_wImportantParameters);
    _lImportantParameters->setSpacing(4);
    _lImportantParameters->setMargin(0);
    _lImportantParameters->setContentsMargins(5,0,5,5);

    _wMidiParameters = new QWidget(_wContent);
    lGroupParameters->addWidget(_wMidiParameters);
    _lMidiParameters = new QVBoxLayout(_wMidiParameters);
    _lMidiParameters->setSpacing(4);
    _lMidiParameters->setMargin(0);
    _lMidiParameters->setContentsMargins(5,0,5,5);
    lGroupParameters->addStretch(1);

    // Connect UI with its group
    connect(_group,SIGNAL(animationAdded()), this,SLOT(updateGroup()));
    connect(_group, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    connect(_group, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    // Set policy
    this->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::MinimumExpanding);

    // Update (delete,recreate) all components within a group
    this->updateGroup();

    // Update UI state of "enabled"
    this->enable(_group->enabled());
}

void UiMasterAnimationGroup::updateGroup()
{
    int cpt = 0;
    foreach (UiAnimationProperty *uiAnimationProperty, this->findChildren<UiAnimationProperty*>())
    {
        disconnect(uiAnimationProperty);
        delete(uiAnimationProperty);
    }
    foreach (QFrame *frame, _wGroupParameters->findChildren<QFrame*>())
    {
        delete(frame);
    }
    foreach (MinoAnimation *animation, _group->animations())
    {
        foreach (MinoPropertyList *list, animation->propertyGrouped())
        {
            foreach (MinoProperty *property, *list)
            {
                MinoMidiControlableProperty *midiControlableProperty = dynamic_cast<MinoMidiControlableProperty*>(property);
                if(midiControlableProperty)
                {
                    connect(midiControlableProperty, SIGNAL(attributesChanged()), this, SLOT(updateGroup()),Qt::UniqueConnection);
                    if(midiControlableProperty->isPreferred())
                    {
                        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wImportantParameters);
                        uiAnimationProperty->setObjectName("animationproperty");
                        connect(animation,SIGNAL(destroyed()),uiAnimationProperty,SLOT(deleteLater()));
                        _lImportantParameters->addWidget(uiAnimationProperty);

                        QFrame *fSeparator = new QFrame(_wImportantParameters);
                        fSeparator->setObjectName("line");
                        fSeparator->setFrameShape(QFrame::HLine);
                        fSeparator->setFrameShadow(QFrame::Sunken);
                        fSeparator->setLineWidth(1);
                        connect(animation,SIGNAL(destroyed()),fSeparator,SLOT(deleteLater()));
                        _lImportantParameters->addWidget(fSeparator);

                        cpt++;
                    } else if (midiControlableProperty->isMidiControlled())
                    {
                        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wMidiParameters);
                        uiAnimationProperty->setObjectName("animationproperty");
                        connect(animation,SIGNAL(destroyed()),uiAnimationProperty,SLOT(deleteLater()));
                        _lMidiParameters->addWidget(uiAnimationProperty);

                        QFrame *fSeparator = new QFrame(_wMidiParameters);
                        fSeparator->setObjectName("line");
                        fSeparator->setFrameShape(QFrame::HLine);
                        fSeparator->setFrameShadow(QFrame::Sunken);
                        fSeparator->setLineWidth(1);
                        connect(animation,SIGNAL(destroyed()),fSeparator,SLOT(deleteLater()));
                        _lMidiParameters->addWidget(fSeparator);

                        cpt++;
                    }
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
    _pbEnable->blockSignals(true);
    _pbEnable->setChecked(on);
    _pbEnable->blockSignals(false);

    this->setProperty("active", on);
    this->style()->unpolish(this);
    this->style()->polish(this);
}

QSize UiMasterAnimationGroup::minimumSizeHint() const
{
    return QSize(80, 50);
}

QSize UiMasterAnimationGroup::sizeHint() const
{
    return QSize(80, 200);
}

