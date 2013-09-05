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

#ifndef MINOITEMIZEDPROPERTY_H
#define MINOITEMIZEDPROPERTY_H

#include "minoproperty.h"
#include "midicontrollablelist.h"

class MinoItemizedProperty : public MinoProperty
{
    Q_OBJECT
public:
    explicit MinoItemizedProperty(QObject *parent);

    void setObjectName(const QString &name);
    void setLabel(const QString &label);

    // Items
    void addItem(const QString &name, const qreal real);
    MidiControllableListItem* currentItem();
    void setCurrentItem(const QString &name);
    void setCurrentItemIndex(const int index);

    // Preferred MIDI control
    void setPreferred(bool on = true) { _mcl->setPreferred(on); }

    // Type: Linear or Items (default)
    void setLinear(bool linear = true);

signals:
    void itemChanged(QString name);
    void itemIdChanged(int id);

private:
    MidiControllableList *_mcl;
};

#endif // MINOITEMIZEDPROPERTY_H
