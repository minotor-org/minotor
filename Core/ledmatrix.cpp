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

#include "ledmatrix.h"

#include <QImage>
#include <QDebug>
#include <QPainter>

LedMatrix::LedMatrix(const QSize size, QObject *parent) :
    QObject(parent),
    _size(size),
    _port(NULL),
    _connected(false),
    _framebuffer()
{
    _port = new QextSerialPort();
}

LedMatrix::~LedMatrix()
{
    if (_connected) _port->close();
    delete _port;
}

bool LedMatrix::openPortByName(const QString& portName)
{
    _port->setPortName(portName);
    _port->setBaudRate(BAUD1000000);
    if (_port->open(QIODevice::WriteOnly)){
        qDebug() << "Led matrix connected to:" << this->portName();
        _connected = true;
        emit(connected());
    } else {
        qDebug() << "Unable to connect to:" << portName;
    }
    return _connected;
}

void LedMatrix::closePort()
{
    if (_port) {
        _port->close();
        qDebug() << "Led matrix disconnected.";
        _connected = false;
        emit(connected(false));
    }
}

bool LedMatrix::isConnected()
{
    return _connected;
}

QString LedMatrix::portName() const
{
    if (_port) {
        return _port->portName();
    }
    return "";
}

void LedMatrix::show(const QImage *image)
{
    const QRgb *pixels = reinterpret_cast<const QRgb*>(image->bits());
    for (unsigned int y=0;y<MATRIX_LEDS_Y;y++)
      {
        for (unsigned int x=0;x<MATRIX_LEDS_X;x++) {
          unsigned int id;

          unsigned int x_panel_id = (y%2)
                           ?((MATRIX_PANEL_LEDS_X-1)-(x%MATRIX_PANEL_LEDS_X))
                           :(x%MATRIX_PANEL_LEDS_X);
          unsigned int y_panel_id = (y%MATRIX_PANEL_LEDS_Y);
          unsigned int panel_id = ((y/MATRIX_PANEL_LEDS_Y)%2)
                         ?((MATRIX_PANEL_X-1)-(x/MATRIX_PANEL_LEDS_X)) + ((y/MATRIX_PANEL_LEDS_Y) * MATRIX_PANEL_X)
                         :(x/MATRIX_PANEL_LEDS_X) + ((y/MATRIX_PANEL_LEDS_Y) * MATRIX_PANEL_X);

          id = x_panel_id + (y_panel_id*MATRIX_PANEL_LEDS_X) + (panel_id*MATRIX_PANEL_LEDS);

          const unsigned int r_id = (id * 3) + 1;
          const unsigned int g_id = (id * 3) + 2;
          const unsigned int b_id = (id * 3) + 0;

            //QRgb rgb(this->_frame->pixel(x, y));
            QRgb rgb = pixels[x+(y*MATRIX_LEDS_X)];
            //qDebug() << "x:" << x << "y:" << y << "color RGB" << qRed(rgb) << qGreen(rgb) << qBlue(rgb);

            _framebuffer[r_id] = (qRed(rgb)==0x01)?0:qRed(rgb);
            _framebuffer[g_id] = (qGreen(rgb)==0x01)?0:qGreen(rgb);
            _framebuffer[b_id] = (qBlue(rgb)==0x01)?0:qBlue(rgb);
        }
    }
    if(_connected)
    {
        _port->write((const char*)_framebuffer,(MATRIX_LEDS*3));
        char endFrame = 0x01;
        _port->write(&endFrame,1);
        // qDebug("frame sent");
    }
    emit(updated());
}

