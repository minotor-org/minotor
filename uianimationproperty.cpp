#include "uianimationproperty.h"

#include <QLayout>
#include <QLabel>
#include <QDebug>
#include <QPainter>

#include "minotor.h"

#include "minoitemizedproperty.h"

UiAnimationProperty::UiAnimationProperty(MinoProperty *property, QWidget *parent) :
    QWidget(parent),
    _midiLearnMode(false)
{
    QVBoxLayout *lProperty = new QVBoxLayout(this);
    lProperty->setSpacing(1);
    lProperty->setMargin(0);
    lProperty->setContentsMargins(0,0,0,0);
    QFrame *fSeparator = new QFrame(this);
    fSeparator->setObjectName("line");
    fSeparator->setFrameShape(QFrame::HLine);
    fSeparator->setFrameShadow(QFrame::Sunken);

    fSeparator->setLineWidth(1);
    lProperty->addWidget(fSeparator);

    MinoItemizedProperty* itemizedProperty = dynamic_cast<MinoItemizedProperty*>(property);
    if(itemizedProperty)
    {
        QLabel *tItemName = new QLabel(itemizedProperty->currentItem()->name(), this);
        tItemName->setAlignment(Qt::AlignHCenter);
        connect(itemizedProperty, SIGNAL(itemChanged(QString)), tItemName, SLOT(setText(QString)));
        lProperty->addWidget(tItemName);
    }

    QWidget *wDial = new QWidget(this);
    lProperty->addWidget(wDial);
    QHBoxLayout *lDial = new QHBoxLayout(wDial);
    lDial->setSpacing(0);
    lDial->setMargin(0);
    lDial->setContentsMargins(0,0,0,0);
    lDial->addStretch();
    _dial = new UiDial(property, this);
    _dial->setMinimumSize(25,25);
    _dial->setMaximumSize(25,25);
    _dial->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(_dial, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
    lDial->addWidget(_dial);
    lDial->addStretch();

    QLabel *t = new QLabel(QString(property->objectName()), this);
    t->setAlignment(Qt::AlignHCenter);
    lProperty->addWidget(t);



}

void UiAnimationProperty::paintEvent(QPaintEvent *pe)
{
    (void)pe;
    static bool midiControlled = _dial->property()->isMidiControlled();
    if(_midiLearnMode)
    {
        if(midiControlled != _dial->property()->isMidiControlled())
        {
            // Tricky way to update full widget region (instead of UiDial only)
            update();
        }

        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        QColor color(127, 127, 127, 127);
        if (this->testAttribute(Qt::WA_UnderMouse))
            color.setGreen(255);
        else
            color.setRed(255);

        if (_dial->property()->isMidiControlled())
        {
            color.setBlue(255);
        }

        painter.setBrush(QBrush(color));
        painter.drawRect(this->rect());
    }
}

void UiAnimationProperty::leaveEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapping()->assignCapturedControlTo(NULL);
        update();
    }
}

void UiAnimationProperty::enterEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapping()->assignCapturedControlTo(_dial->property());
        update();
    }
}
