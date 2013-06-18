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

#ifndef MINOPROGRAMBANK_H
#define MINOPROGRAMBANK_H

#include <QList>
#include "minopersistentobject.h"
#include "minoprogram.h"
class Minotor;

class MinoProgramBank : public MinoPersistentObject
{
    Q_OBJECT
public:
    explicit MinoProgramBank(QObject *parent);
    // Program
    void addProgram(MinoProgram *program);
    QList<MinoProgram*> programs() { return _programs; }
    ~MinoProgramBank();
    Minotor *minotor();

signals:
    void programAdded(QObject *program);
    void programSelectorChanged(QObject *program);

private:
    // Programs
    QList<MinoProgram*> _programs;
    int _programSelectorPos;
    
public slots:

protected slots:
    void destroyProgram(QObject *program);

    void programSelectorNext();
    void programSelectorPrevious();
    void programSelectorSelect();
};

#endif // MINOPROGRAMBANK_H
