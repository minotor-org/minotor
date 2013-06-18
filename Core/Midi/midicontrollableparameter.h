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

#ifndef MIDICONTROLLABLEPARAMETER_H
#define MIDICONTROLLABLEPARAMETER_H

#include <QObject>
#include "minopersistentobject.h"

class MidiControllableParameter : public MinoPersistentObject
{
    friend class MidiMapper;
    Q_OBJECT
    Q_PROPERTY(bool preferred READ isPreferred WRITE setPreferred STORED true)
public:
    enum Attribute {
        None = 0x0,
        MidiControled = 0x1,
        Preferred = 0x2
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    explicit MidiControllableParameter(QObject *parent = 0);

    void setPreferred(bool on = true);
    bool isPreferred() { return attributes().testFlag(MidiControllableParameter::Preferred); }
    bool isMidiControlled() { return attributes().testFlag(MidiControllableParameter::MidiControled); }
    quint8 valueFromMidi() { return _value; }

    // Attributes
    MidiControllableParameter::Attributes attributes() { return _attributes; }
    void setAttributes(MidiControllableParameter::Attributes attributes);

    // Label
    void setLabel(const QString& label) { _label = label; }
    QString label() { return _label; }

public slots:
    virtual void setValueFromMidi(quint8 value);

signals:
    void valueFromMidiChanged(quint8 value);
    void attributesChanged();

private:
    quint8 _value;
    MidiControllableParameter::Attributes _attributes;
    QString _label;

public slots:
    
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MidiControllableParameter::Attributes)

#endif // MIDICONTROLLABLEPARAMETER_H
