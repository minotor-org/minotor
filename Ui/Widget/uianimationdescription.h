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

#ifndef UIANIMATIONDESCRIPTION_H
#define UIANIMATIONDESCRIPTION_H

#include <QLabel>

#include "minoanimation.h"

class UiAnimationDescription : public QLabel
{
    Q_OBJECT
public:
    explicit UiAnimationDescription(const MinoAnimationDescription& animation, QWidget *parent);

    MinoAnimationDescription *description() { return &_animation; }
signals:
    
public slots:
    
private:
    MinoAnimationDescription _animation;
};

#endif // UIANIMATIONDESCRIPTION_H
