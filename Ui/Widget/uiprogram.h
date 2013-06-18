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

#ifndef UIPROGRAM_H
#define UIPROGRAM_H

#include <QWidget>
#include <QFrame>
#include "minoprogram.h"

class UiProgram : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgram(MinoProgram *program, QWidget *parent);
    void setOnAir(bool onAir);
    void setHighlight(bool on);
    MinoProgram *program() { return _program; }

private:
    MinoProgram *_program;
    QFrame *_fPreview;
    QWidget *_wBackground;
    QWidget *_wBorder;
    bool _highlight;
    QWidget *_wHighlight;

signals:

public slots:
    void setExpanded(bool expanded);
private slots:
    void updateOnAirStatus(bool onAir);
    void requestMasterProgramChange(bool on);
    void exportProgram();
    void confirmDelete();
};

#endif // UIPROGRAM_H
