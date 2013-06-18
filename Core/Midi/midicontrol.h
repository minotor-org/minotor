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

#ifndef MIDICONTROL_H
#define MIDICONTROL_H

#include <QObject>

class MidiControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint8 interface READ interface STORED true)
    Q_PROPERTY(quint8 channel READ channel STORED true)
    Q_PROPERTY(quint8 control READ control STORED true)
public:
    explicit MidiControl(int interface, quint8 channel, quint8 control, QObject *parent = 0);
    void setValue(quint8 value);
    int interface() const { return _interface; }
    quint8 channel() const { return _channel; }
    quint8 control() const { return _control; }
    quint8 value() const { return _value; }

protected:
    int _interface;
    quint8 _channel;
    quint8 _control;
    quint8 _value;

signals:
    void valueChanged(quint8 value);

public slots:
    
};

typedef QList<MidiControl*> MidiControlList;

#endif // MIDICONTROL_H
