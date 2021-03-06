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

#ifndef UIMIDIINTERFACE_H
#define UIMIDIINTERFACE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

#include "midiinterface.h"

class UiMidiInterface : public QWidget
{
    Q_OBJECT
public:
    explicit UiMidiInterface(MidiInterface *interface, QWidget *parent = 0);

    QString name() { return _txtName->text(); }
    void setName(QString name) { _txtName->setText(name); }
signals:
    
public slots:

private slots:
    void mappingChanged(int);

private:
    QLabel *_txtName;
    QComboBox *_cbMapping;
    MidiInterface *_interface;
};

#endif // UIMIDIINTERFACE_H
