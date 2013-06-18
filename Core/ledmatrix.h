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

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsView>

#include "qextserialport.h"

#define MATRIX_PANEL_X         3
#define MATRIX_PANEL_Y         2
#define MATRIX_PANEL_LEDS_X    8
#define MATRIX_PANEL_LEDS_Y    8
#define MATRIX_PANEL_LEDS      (MATRIX_PANEL_LEDS_X*MATRIX_PANEL_LEDS_Y)
#define MATRIX_LEDS_X          (MATRIX_PANEL_X*MATRIX_PANEL_LEDS_X)
#define MATRIX_LEDS_Y          (MATRIX_PANEL_Y*MATRIX_PANEL_LEDS_Y)
#define MATRIX_LEDS            (MATRIX_LEDS_X*MATRIX_LEDS_Y)

class LedMatrix : public QObject
{
     Q_OBJECT
public:
    explicit LedMatrix(QObject *parent = 0);
    ~LedMatrix();

    bool openPortByName(const QString &portName);
    void closePort();
    QString portName() const;

    bool isConnected();

    void show(const QImage *image);
    void showScene(QGraphicsScene * scene);

    // Accessors
    QSize size() const { return _size; }
    QRect rect() const { return QRect(QPoint(0,0), _size); }

private:
    QSize _size;
    QextSerialPort *_port;
    bool _connected;

    unsigned char _framebuffer[MATRIX_LEDS*3];
signals:
    void updated();
    void connected(bool connected = true);
public slots:
    
};

#endif // LEDMATRIX_H
