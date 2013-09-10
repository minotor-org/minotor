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

#ifndef MINOPROPERTYBEAT_H
#define MINOPROPERTYBEAT_H

#include "minoproperty.h"
#include "midicontrollablelist.h"

class MinoPropertyBeat : public MinoProperty
{
    Q_OBJECT
    Q_PROPERTY (QString loop_size READ loopSize WRITE setLoopSize STORED true)

public:
    explicit MinoPropertyBeat(QObject *parent = 0);

    void setObjectName(const QString &name);
    
    void setLoopSize(const QString& loopSize) { _mcl->setCurrentItemFromString(loopSize); }
    QString loopSize() const { return _mcl->currentItem()->name(); }
    qreal loopSizeInPpqn() const { return _mcl->currentItem()->real(); }

    qreal progressForGppqn(const unsigned int gppqn) const;
    bool isBeat(const unsigned int gppqn) const;

signals:
    void loopSizeChanged(const QString& loopSize);

private:
    MidiControllableList *_mcl;
};

#endif // MINOPROPERTYBEAT_H
