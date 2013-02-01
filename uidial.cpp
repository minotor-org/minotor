#include "uidial.h"

#include <QDebug>

#include <QPainter>

#include "minotor.h"

UiDial::UiDial(MinoAnimationProperty *property, QWidget *parent) :
    QDial(parent),
    _property(property),
    _midiLearnMode(false)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(property, SIGNAL(valueChanged(qreal)), this, SLOT(setValueFromProperty(qreal)));
    this->setValueFromProperty(property->value());
    this->setMaximum(127);
    this->setMinimum(0);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(setAnimationPropertyValue(int)));
    this->setMaximumHeight(40);
}

void UiDial::setValueFromProperty(qreal value)
{
    this->setValue(value*127);
}

void UiDial::setAnimationPropertyValue(int value)
{
    _property->setValue(((qreal)value)/127);
}

void UiDial::paintEvent(QPaintEvent *pe)
{
    QDial::paintEvent(pe);
    if(_midiLearnMode)
    {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        QColor color(127, 127, 127, 127);
        if (this->testAttribute(Qt::WA_UnderMouse))
            color.setGreen(255);
        else
            color.setRed(255);

        if (_property->isMidiControlled())
        {
            color.setBlue(255);
        }

        painter.setBrush(QBrush(color));
        painter.drawRect(this->rect());
    }
}

void UiDial::leaveEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapping()->assignCapturedControlTo(NULL);
        update();
    }
}

void UiDial::enterEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapping()->assignCapturedControlTo(this->property());
        update();
    }
}
