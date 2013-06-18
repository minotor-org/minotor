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

#ifndef UIEASINGCURVE_H
#define UIEASINGCURVE_H

#include <QWidget>

#include <QEasingCurve>

class UiEasingCurve : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor axes_color READ axesColor WRITE setAxesColor DESIGNABLE true)
    Q_PROPERTY(QColor grid_color READ gridColor WRITE setGridColor DESIGNABLE true)
    Q_PROPERTY(QColor curve_color READ curveColor WRITE setCurveColor DESIGNABLE true)
public:
    explicit UiEasingCurve(QWidget *parent = 0);
    
    QColor axesColor() { return _axesColor; }
    void setAxesColor(QColor color) { _axesColor = color; }

    QColor gridColor() { return _gridColor; }
    void setGridColor(QColor color) { _gridColor = color; }

    QColor curveColor() { return _curveColor; }
    void setCurveColor(QColor color) { _curveColor = color; }

signals:
    
public slots:
    void setEasingCurve(const QEasingCurve& ec);

protected:
    void paintEvent(QPaintEvent *pe);

    QColor _axesColor;
    QColor _gridColor;
    QColor _curveColor;
private:
    QEasingCurve _ec;
};

#endif // UIEASINGCURVE_H
