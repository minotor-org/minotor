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

#ifndef MINOCONTROL_H
#define MINOCONTROL_H

#include <QObject>

class MinoControl : public QObject
{
    Q_OBJECT
public:
    explicit MinoControl(QString role, QObject *parent = 0);
    
    void setValue(quint8 value);

    QString role() { return _role; }
signals:
    void valueChanged(quint8);

public slots:
    
protected:
    quint8 _value;
    QString _role;
};

#endif // MINOCONTROL_H
