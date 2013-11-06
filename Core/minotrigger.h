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

#ifndef MINOTRIGGER_H
#define MINOTRIGGER_H

#include <QObject>

class MinoTrigger : public QObject
{
    Q_OBJECT
public:
    explicit MinoTrigger(QString role, QObject *parent = 0);
    
    void setStatus(bool on);

    QString role() const { return _role; }
signals:
    void toogled(bool on);
    void triggered();
    void feedback(bool on);

public slots:
    
protected:
    bool _on;
    QString _role;
};

#endif // MINOTRIGGER_H
