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

#ifndef UIANIMATION_H
#define UIANIMATION_H

#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>

#include "minoanimation.h"

class UiAnimation : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiAnimation(MinoAnimation *animation, QWidget *parent);
    void setExpanded(bool expanded);
    MinoAnimation *animation() { return _animation; }

    template<typename T> T findParent();

protected:
    QLabel *_tAnimation;

private:
    QWidget *_wProperties;
    MinoAnimation *_animation;

public slots:

protected slots:
    void mousePressEvent(QMouseEvent *event);

    void takeAShot();

};

#endif // UIANIMATION_H
