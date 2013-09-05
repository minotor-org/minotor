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

#ifndef MIDICONTROLLABLELIST_H
#define MIDICONTROLLABLELIST_H

#include "midicontrollableparameter.h"

class MidiControllableListItem
{
public:
    explicit MidiControllableListItem(const QString name, const qreal real):
        _name(name),
        _real(real)
    {}

    QString name() const { return _name; }
    qreal real() const { return _real; }

private:
    QString _name;
    qreal _real;
};

class MidiControllableList : public MidiControllableParameter
{
    Q_OBJECT
public:
    explicit MidiControllableList(QObject *parent = 0);
    ~MidiControllableList();

    // Type
    enum Type { Items, Steps };
    MidiControllableList::Type type() { return _type; }
    void setType(MidiControllableList::Type type) { _type = type; }

    unsigned int itemsCount() { return _items.count(); }

    // Items
    void addItem(const QString name, const qreal real);
    MidiControllableListItem* currentItem();
    void setCurrentItem(const QString &name);
    void setCurrentItemIndex(const int index);
    int currentItemIndex() { return _currentItemId; }

    // Type: Linear or Items (default)
    void setLinear(bool linear = true);

private:
    MidiControllableList::Type _type;
    QList<MidiControllableListItem*> _items;
    int _currentItemId;

signals:
    void itemChanged(QString name);
    void itemIdChanged(int id);
    void itemValueChanged(qreal value);

public slots:
    void setValueFromMidi(quint8 value);
};

#endif // MIDICONTROLLABLELIST_H
