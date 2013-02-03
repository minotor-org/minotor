#include "uianimationproperty.h"

#include <QLayout>
#include <QLabel>
#include <QDebug>
#include <QPainter>

#include "minotor.h"

UiAnimationProperty::UiAnimationProperty(MinoAnimationProperty *property, QWidget *parent) :
    QWidget(parent),
    _midiLearnMode(false)
{
    QVBoxLayout *lProperty = new QVBoxLayout(this);
    _dial = new UiDial(property, this);
    _dial->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(_dial, SIGNAL(customContextMenuRequested(QPoint)), this, SIGNAL(customContextMenuRequested(QPoint)));
    lProperty->addWidget(_dial);
    QLabel *t = new QLabel(QString(property->objectName()), this);
    t->setAlignment(Qt::AlignCenter);
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
