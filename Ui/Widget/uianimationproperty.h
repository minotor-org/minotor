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

#ifndef UIANIMATIONPROPERTY_H
#define UIANIMATIONPROPERTY_H

#include <QWidget>

#include "minoproperty.h"

class UiAnimationProperty : public QWidget
{
    Q_OBJECT
public:
    explicit UiAnimationProperty(MinoProperty *property, QWidget *parent, bool editorMode = false);

    int	columnCount () const;

protected:

private:
    int _columnCount;

signals:
    
public slots:

};

#endif // UIANIMATIONPROPERTY_H
