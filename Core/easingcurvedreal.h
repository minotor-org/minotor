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

#ifndef EASINGCURVEDREAL_H
#define EASINGCURVEDREAL_H

#include <QObject>

#include <QEasingCurve>

class EasingCurvedReal : public QObject
{
    Q_OBJECT
public:
    explicit EasingCurvedReal(QObject *parent = 0);
    
    void setStartValue(const qreal start) { _start = start; }
    void setEndValue(const qreal end) { _end = end; }
    void setEasingCurve(const QEasingCurve& ec) { _ec = ec; }
    qreal valueForProgress(const qreal progress) { return ((_ec.valueForProgress(progress) * (_end-_start)) + _start); }

signals:
    
public slots:

private:
    qreal _start;
    qreal _end;
    QEasingCurve _ec;
    
};

#endif // EASINGCURVEDREAL_H
