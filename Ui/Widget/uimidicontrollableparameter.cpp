/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uimidicontrollableparameter.h"

#include <QPainter>
#include <QColor>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

#include "minotor.h"

#include "minoitemizedproperty.h"

#include "uiknob.h"

UiMidiControllableParameter::UiMidiControllableParameter(MidiControllableParameter *parameter, QWidget *parent, bool editorMode) :
    QWidget(parent),
    _midiLearnMode(false),
    _midiControlled(false),
    _parameter(parameter)
{
    this->setMinimumWidth(50);
    this->setMinimumHeight(50);
    this->setMaximumWidth(50);
    this->setMaximumHeight(50);

    QVBoxLayout *lProperty = new QVBoxLayout(this);
    lProperty->setSpacing(2);
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

    MidiControllableList* mcl = dynamic_cast<MidiControllableList*>(parameter);
    if(mcl)
    {
        if(editorMode)
        {
            QWidget *wLeft = new QWidget(wTop);
            wLeft->setMinimumSize(6,6);
            wLeft->setMaximumSize(6,6);
            lTop->addWidget(wLeft);
            lTop->addStretch();

            QString itemName = "no item";
            if(mcl->currentItem())
                itemName = mcl->currentItem()->name();
            QLabel *tItemName = new QLabel(itemName, wTop);
            tItemName->setObjectName("dialinfo");
            tItemName->setAlignment(Qt::AlignHCenter);
            connect(mcl, SIGNAL(itemChanged(QString)), tItemName, SLOT(setText(QString)));
            lTop->addWidget(tItemName);
            lTop->addStretch();

            QPushButton *pbOnMaster = new QPushButton(wTop);
            pbOnMaster->setObjectName("tiny");
            pbOnMaster->setCheckable(true);
            pbOnMaster->setChecked(mcl->isPreferred());
            pbOnMaster->setFixedSize(6,6);
            lTop->addWidget(pbOnMaster);
            connect(pbOnMaster,SIGNAL(toggled(bool)), this, SLOT(togglePropertyToMaster(bool)));
        }
        else
        {
            lTop->addStretch();
            QLabel *tItemName = new QLabel(mcl->currentItem()->name(), wTop);
            tItemName->setObjectName("dialinfo");
            tItemName->setAlignment(Qt::AlignHCenter);
            connect(mcl, SIGNAL(itemChanged(QString)), tItemName, SLOT(setText(QString)));
            lTop->addWidget(tItemName);
            lTop->addStretch();
        }
    }
    else
    {
        if(editorMode)
        {
            lTop->addStretch();
            QPushButton *pbOnMaster = new QPushButton(wTop);
            pbOnMaster->setObjectName("tiny");
            pbOnMaster->setCheckable(true);
            pbOnMaster->setChecked(parameter->isPreferred());
            pbOnMaster->setFixedSize(6,6);
            lTop->addWidget(pbOnMaster);
            connect(pbOnMaster,SIGNAL(toggled(bool)), this, SLOT(togglePropertyToMaster(bool)));
        }
    }

    QWidget *wDial = new QWidget(this);
    lProperty->addWidget(wDial);
    QHBoxLayout *lDial = new QHBoxLayout(wDial);
    lDial->setSpacing(0);
    lDial->setMargin(0);
    lDial->setContentsMargins(0,0,0,0);
    lDial->addStretch();
    UiKnob* knob = new UiKnob(parameter, wDial);
    lDial->addWidget(knob);
    lDial->addStretch();

    QLabel *t = new QLabel(QString(parameter->label()), this);
    t->setObjectName("dialinfo");
    t->setAlignment(Qt::AlignHCenter);
    lProperty->addWidget(t);

    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}

void UiMidiControllableParameter::paintEvent(QPaintEvent *pe)
{
    (void)pe;
    if(_midiLearnMode)
    {
        if(_midiControlled != _parameter->isMidiControlled())
        {
            _midiControlled = _parameter->isMidiControlled();
            // Tricky way to update full widget region (instead of UiKnob only)
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

        if (_parameter->isMidiControlled())
        {
            color.setBlue(255);
        }

        painter.setBrush(QBrush(color));
        painter.drawRect(this->rect());
    }
    else
    {
        _midiControlled = _parameter->isMidiControlled();
    }
}

void UiMidiControllableParameter::leaveEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapper()->assignCapturedControlTo(NULL);
        update();
    }
}

void UiMidiControllableParameter::enterEvent(QEvent *event)
{
    (void)event;
    if (_midiLearnMode)
    {
        Minotor::minotor()->midiMapper()->assignCapturedControlTo(_parameter);
        update();
    }
}

void UiMidiControllableParameter::togglePropertyToMaster(bool on)
{
    _parameter->setPreferred(on);
}

QSize UiMidiControllableParameter::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize UiMidiControllableParameter::sizeHint() const
{
    return QSize(50, 50);
}

void UiMidiControllableParameter::setMidiLearnMode(bool on)
{
    _midiLearnMode = on;
    update();
}
