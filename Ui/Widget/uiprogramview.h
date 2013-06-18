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

#ifndef UIPROGRAMVIEW_H
#define UIPROGRAMVIEW_H

#include <QWidget>

#include "minoprogram.h"

class UiProgramView : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramView(MinoProgram *program, QWidget *parent);
signals:
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);

    virtual int heightForWidth( int width ) const;
public slots:
    void setProgram(MinoProgram *program);
protected slots:
    void clear();
private:
    MinoProgram *_program;
    QVector<QLine> _gridLines;
    qreal _gridStepMin;

};

#endif // UIPROGRAMVIEW_H
