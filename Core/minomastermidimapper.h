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

#ifndef MINOMASTERMIDIMAPPER_H
#define MINOMASTERMIDIMAPPER_H

#include <QObject>

class MinoMaster;
class MinoProgram;
class MinoAnimationGroup;
class MinoAnimation;

class MinoMasterMidiMapper : public QObject
{
    Q_OBJECT
public:
    explicit MinoMasterMidiMapper(MinoMaster *parent);

signals:
    void updated();

public slots:
    void updateProgram();

protected slots:
    void addGroup(QObject *group);
    void moveGroup(QObject *group);

    void addAnimation(QObject *animation);
    void moveAnimation(QObject *animation);

    void changeMidiControllableParameterAttributes();

    void incrementVirtualPageOffset();
    void decrementVirtualPageOffset();

protected:
    MinoMaster *_master;
    MinoProgram *_program;

    int _knobsPerTrack;
    int _virtualPageWidth;
    int _virtualPageOffset;

    void registerProgram(MinoProgram *mp);
    void unregisterProgram(MinoProgram *mp);

    void registerGroup(MinoAnimationGroup *mag);
    void unregisterGroup(MinoAnimationGroup *mag);

    void registerAnimation(MinoAnimation *ma);
    void unregisterAnimation(MinoAnimation *ma);

    void registerRoles();
    void clearRoles();

    void updateMap();

    void setVirtualPageOffset(int offset);
};

#endif // MINOMASTERMIDIMAPPER_H
