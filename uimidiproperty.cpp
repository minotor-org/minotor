#include "uimidiproperty.h"

#include <QPainter>
#include <QColor>
#include <QLayout>

#include "minotor.h"

UiMidiProperty::UiMidiProperty(MinoProperty *property, QWidget *parent) :
    QWidget(parent),
    _midiLearnMode(false),
    _knob(NULL),
    _property(property)
{
    QHBoxLayout *lDial = new QHBoxLayout(this);
    lDial->setSpacing(0);
    lDial->setMargin(0);
    lDial->setContentsMargins(0,0,0,0);
    _knob = new UiKnob(property, this);
    lDial->addWidget(_knob);
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
