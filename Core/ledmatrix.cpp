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
    _connected(false)
{
    _port = new QextSerialPort();

    prepareFramebuffer();
    computeLookUpTable();
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
        qDebug() << "Led matrix failed to connect to:" << portName;
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

bool LedMatrix::isConfigured() const
{
    return _size.isValid();
}

void LedMatrix::prepareFramebuffer()
{
    if(isConfigured())
    {
        _framebuffer.resize(_size.width()*_size.height()*3);
    }
}

void LedMatrix::computeLookUpTable()
{
    if(isConfigured())
    {
        const int width = _size.width();
        const int height = _size.height();

        _pixelsLUT.clear();
        _pixelsLUT.resize(height*width);

        for (int y=0;y<height;y++)
        {
            for (int x=0;x<width;x++) {
                const unsigned int x_panel_id = (y%2)
                        ?((MATRIX_PANEL_LEDS_X-1)-(x%MATRIX_PANEL_LEDS_X))
                       :(x%MATRIX_PANEL_LEDS_X);
                const unsigned int y_panel_id = (y%MATRIX_PANEL_LEDS_Y);
                const unsigned int panel_id = ((y/MATRIX_PANEL_LEDS_Y)%2)
                        ?((MATRIX_PANEL_X-1)-(x/MATRIX_PANEL_LEDS_X)) + ((y/MATRIX_PANEL_LEDS_Y) * MATRIX_PANEL_X)
                       :(x/MATRIX_PANEL_LEDS_X) + ((y/MATRIX_PANEL_LEDS_Y) * MATRIX_PANEL_X);

                const unsigned int id = x_panel_id + (y_panel_id*MATRIX_PANEL_LEDS_X) + (panel_id*MATRIX_PANEL_LEDS);
                const unsigned int i = x+(y*width);
                Q_ASSERT(i<(unsigned int)_pixelsLUT.size());
                _pixelsLUT.data()[i] = id;
            }
        }
    }
}

void LedMatrix::show(const QImage *image)
{
    const unsigned int matrix_width = _size.width();
    const unsigned int matrix_height = _size.height();
    const QRgb *pixels = reinterpret_cast<const QRgb*>(image->bits());

    for (unsigned int y=0;y<matrix_height;y++)
    {
        for (unsigned int x=0;x<matrix_width;x++) {
            const unsigned int id = _pixelsLUT.data()[x+(y*_size.width())];

            const unsigned int r_id = (id * 3) + 1;
            const unsigned int g_id = (id * 3) + 2;
            const unsigned int b_id = (id * 3) + 0;

            QRgb rgb = pixels[x+(y*MATRIX_LEDS_X)];

            char *framebuffer = _framebuffer.data();
            framebuffer[r_id] = (qRed(rgb)==0x01)?0:qRed(rgb);
            framebuffer[g_id] = (qGreen(rgb)==0x01)?0:qGreen(rgb);
            framebuffer[b_id] = (qBlue(rgb)==0x01)?0:qBlue(rgb);
        }
    }
    if(_connected)
    {
        _port->write(_framebuffer.constData(),_framebuffer.size());
        char endFrame = 0x01;
        _port->write(&endFrame,1);
    }
    emit(updated());
}

