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

#include "minoprogrambank.h"
#include "minotor.h"
#include <QDebug>

MinoProgramBank::MinoProgramBank(QObject *parent) :
    MinoPersistentObject(parent),
    _programSelectorPos(-1)
{
    MidiMapper::registerTrigger("BANK_NEXT", "Move selector to next program", this, SLOT(programSelectorNext()), NULL, NULL, MinoRole::Trigger, true);
    MidiMapper::registerTrigger("BANK_PREVIOUS", "Move selector to previous program", this, SLOT(programSelectorPrevious()), NULL, NULL, MinoRole::Trigger, true);
    MidiMapper::registerTrigger("BANK_SELECT", "Set selected program as master", this, SLOT(programSelectorSelect()), NULL, NULL, MinoRole::Trigger, true);
}


void MinoProgramBank::addProgram(MinoProgram *program)
{
    _programSelectorPos = -1;
    emit programSelectorChanged(NULL);

    _programs.append(program);
    const int id = _programs.indexOf(program);

    // Program ID starts at 1
    program->setId(id+1);

    LedMatrix *matrix = minotor()->ledMatrix();
    // Inform program about matrix size (used by animations)
    program->setRect(matrix->rect());
    // Drawing rect
    // On the scene, the program have a dedicated area to display/draw animations
    // This area left one "screen" before and one "screen" areas to prevent from collisions
    // Note: Developer of animations should take care to not collide: its objects should never be larger than one screen-size in all directions (up, down, left, right, diagonals)
    QPointF pos = QPointF(matrix->size().width()*3, matrix->size().height() + ((matrix->size().height()*3) * id));
    program->setDrawingPos(pos);
    connect(program,SIGNAL(destroyed(QObject*)),this,SLOT(destroyProgram(QObject*)));
    emit programAdded(program);
}

MinoProgramBank::~MinoProgramBank()
{
    foreach(MinoProgram *prg, _programs)
    {
        delete prg;
    }
    _programs.clear();
}

Minotor* MinoProgramBank::minotor()
{
    Minotor* minotor =  qobject_cast<Minotor*>(parent());
    Q_ASSERT(minotor);
    return minotor;
}

void MinoProgramBank::destroyProgram(QObject *program)
{    
    _programs.removeAt(_programs.indexOf(static_cast<MinoProgram*>(program)));
    _programSelectorPos = -1;
    emit programSelectorChanged(NULL);
}

void MinoProgramBank::programSelectorNext()
{
    if (++_programSelectorPos >= _programs.count() )
    {
        _programSelectorPos = 0;
    }
    Q_ASSERT(_programs.at(_programSelectorPos));
    emit programSelectorChanged(_programs.at(_programSelectorPos));
}

void MinoProgramBank::programSelectorPrevious()
{
    if (--_programSelectorPos < 0)
    {
        _programSelectorPos = _programs.count() - 1;
    }
    Q_ASSERT(_programs.at(_programSelectorPos));
    emit programSelectorChanged(_programs.at(_programSelectorPos));
}

void MinoProgramBank::programSelectorSelect()
{
    if((_programSelectorPos >= 0)
            && (_programSelectorPos < _programs.count()))
    {
        Minotor::minotor()->master()->setProgram(_programs.at(_programSelectorPos));
    }
}
