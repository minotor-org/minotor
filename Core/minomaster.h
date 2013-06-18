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

#ifndef MINOMASTER_H
#define MINOMASTER_H

#include "minoprogram.h"
#include "minomastermidimapper.h"

class Minotor;

class MinoMaster : public QObject
{
    Q_OBJECT
public:
    explicit MinoMaster(Minotor *minotor);
    ~MinoMaster();

    void setProgram(MinoProgram *program);
    MinoProgram *program() { return _program; }

    void setViewportRange(const int min, const int max);

private:
    MinoProgram *_program;
    MinoMasterMidiMapper *_midiMapper;

    bool _shifted;
    QGraphicsItemGroup _itemGroup;

signals:
    void programChanged();
    void viewportRangeChanged(const int min, const int max);
    void updated();

public slots:
    void setBrightness(qreal value);
    void clear();
};

#endif // MINOMASTER_H
