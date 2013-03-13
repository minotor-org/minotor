#include "uimidiproperty.h"

#include <QPainter>
#include <QColor>
#include <QLayout>
#include <QLabel>
#include <QPushButton>

#include "minotor.h"

#include "minoitemizedproperty.h"

#include "uiknob.h"

UiMidiProperty::UiMidiProperty(MinoProperty *property, QWidget *parent) :
    QWidget(parent),
    _midiLearnMode(false),
    _property(property)
{
    QVBoxLayout *lProperty = new QVBoxLayout(this);
    lProperty->setSpacing(0);
    lProperty->setMargin(0);
    lProperty->setContentsMargins(0,0,0,0);
    lProperty->addStretch();

    QWidget *wTop = new QWidget(this);
    lProperty->addWidget(wTop);
    wTop->setFixedHeight(12);
    QHBoxLayout *lTop = new QHBoxLayout(wTop);
    lTop->setSpacing(0);
    lTop->setMargin(0);
    lTop->setContentsMargins(2,0,2,0);
    QWidget *wLeft = new QWidget(wTop);
    wLeft->setMinimumSize(6,6);
    wLeft->setMaximumSize(6,6);
    lTop->addWidget(wLeft);
    lTop->addStretch();

    MinoItemizedProperty* itemizedProperty = dynamic_cast<MinoItemizedProperty*>(property);
    if(itemizedProperty)
    {
        QLabel *tItemName = new QLabel(itemizedProperty->currentItem()->name(), wTop);
        tItemName->setObjectName("dialinfo");
        tItemName->setAlignment(Qt::AlignHCenter);
        connect(itemizedProperty, SIGNAL(itemChanged(QString)), tItemName, SLOT(setText(QString)));
        lTop->addWidget(tItemName);
    }
    lTop->addStretch();
    QPushButton *pbOnMaster = new QPushButton(wTop);
    pbOnMaster->setObjectName("tiny");
    pbOnMaster->setCheckable(true);
    pbOnMaster->setFixedSize(6,6);
    lTop->addWidget(pbOnMaster);
    connect(pbOnMaster,SIGNAL(toggled(bool)), this, SLOT(togglePropertyToMaster(bool)));

    QWidget *wDial = new QWidget(this);
    lProperty->addWidget(wDial);
    QHBoxLayout *lDial = new QHBoxLayout(wDial);
    lDial->setSpacing(0);
    lDial->setMargin(0);
    lDial->setContentsMargins(0,0,0,0);
    lDial->addStretch();
    UiKnob* knob = new UiKnob(property, wDial);
    lDial->addWidget(knob);
    lDial->addStretch();

    QLabel *t = new QLabel(QString(property->objectName()), this);
    t->setObjectName("dialinfo");
    t->setAlignment(Qt::AlignHCenter);
    lProperty->addWidget(t);

    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void UiMidiProperty::paintEvent(QPaintEvent *pe)
{
    (void)pe;
    static bool midiControlled = _property->isMidiControlled();
    if(_midiLearnMode)
    {
        if(midiControlled != _property->isMidiControlled())
        {
            // Tricky way to update full widget region (instead of UiDial only)
            update();
        }

        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        QColor color(127, 127, 127, 80);
        if (this->testAttribute(Qt::WA_UnderMouse))
        {
            color.setGreen(255);
        }
        else
        {
            color.setRed(230);
            color.setGreen(95);
            color.setBlue(0);
        }

        if (_property->isMidiControlled())
        {
            color.setBlue(255);
        }

        painter.setBrush(QBrush(color));
        painter.drawRect(this->rect());
    }
}

void UiMidiProperty::leaveEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapping()->assignCapturedControlTo(NULL);
        update();
    }
}

void UiMidiProperty::enterEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapping()->assignCapturedControlTo(_property);
        update();
    }
}

void UiMidiProperty::togglePropertyToMaster(bool on)
{
    if (on)
    {
        _property->setAttributes(_property->attributes() | MinoProperty::Important);
    }
    else
    {
        _property->setAttributes(_property->attributes() & ~MinoProperty::Attributes(MinoProperty::Important));
    }
}

QSize UiMidiProperty::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize UiMidiProperty::sizeHint() const
{
    return QSize(50, 50);
}
