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

#ifndef UIMIDICONTROLLABLEPARAMETER_H
#define UIMIDICONTROLLABLEPARAMETER_H

#include <QWidget>

#include "midicontrollableparameter.h"

class UiMidiControllableParameter : public QWidget
{
    Q_OBJECT
public:
    explicit UiMidiControllableParameter(MidiControllableParameter *parameter, QWidget *parent, bool editorMode = false);
    void setMidiLearnMode(bool on = true);

protected:
    void paintEvent(QPaintEvent *pe);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    bool _midiLearnMode;
    bool _midiControlled;

    MidiControllableParameter *parameter;

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

private:
    MidiControllableParameter *_parameter;
signals:
    
public slots:
    
private slots:
    void togglePropertyToMaster(bool on);
};

#endif // UIMIDICONTROLLABLEPARAMETER
