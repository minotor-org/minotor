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

#include "uiprogramview.h"

#include <QPainter>
#include <QLine>
#include <QDebug>

#include "minotor.h"

UiProgramView::UiProgramView(MinoProgram *program, QWidget *parent) :
    QWidget(parent),
    _program(NULL),
    _gridStepMin(0)
{
    // Optimize widget's repaint
    setAttribute(Qt::WA_OpaquePaintEvent);

    setProgram(program);
}

void UiProgramView::resizeEvent(QResizeEvent *)
{
    const QRect rect = Minotor::minotor()->displayRect();
    _gridLines.clear();
    const qreal stepX = (qreal)width() / rect.width();
    const qreal stepY = (qreal)height() / rect.height();
    for (int x = 1; x < rect.width(); x++)
    {
        int pos = x * stepX;
        _gridLines.append(QLine(pos,0,pos,height()));
    }
    for (int y = 1; y < rect.height(); y++)
    {
        int pos = y * stepY;
        _gridLines.append(QLine(0,pos,width(),pos));
    }
    _gridStepMin = qMin(stepX, stepY);
}

// This function produce draw the widget content
//   It draw the render content (scaled to fit to widget size) and decorates it using some vertical and horizotal black lines
void UiProgramView::paintEvent(QPaintEvent *event)
{
    // event is not used
    (void)event;

    if(!_program)
        return;

    // Make your code cleaner: store variables from renderer as local const
    const QImage *rendering = _program->rendering();

    // Construct a painter to draw into this widget
    QPainter painter(this);

    painter.drawImage(rect(), *rendering, rendering->rect());

    QPen pen;
    pen.setWidthF(_gridStepMin*0.25);
    pen.setColor(Qt::black);
    painter.setPen(pen);

    painter.drawLines(_gridLines);
}

int UiProgramView::heightForWidth( int width ) const
{
    return (_program->heightForWidth(width));
}

void UiProgramView::setProgram(MinoProgram *program)
{
    if(_program)
    {
        disconnect(_program, SIGNAL(animated()), this, SLOT(update()));
        disconnect(_program, SIGNAL(destroyed()), this, SLOT(clear()));
    }
    if(program)
    {
        connect(program, SIGNAL(animated()), this, SLOT(update()));
        connect(program, SIGNAL(destroyed()), this, SLOT(clear()));
    }
    _program = program;
}

void UiProgramView::clear()
{
    _program = NULL;
}
