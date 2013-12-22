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

LedMatrix::LedMatrix(const QSize size, const QSize panelSize, const QSize matrixSize, QObject *parent) :
    QObject(parent),
    _panelSize(panelSize),
    _matrixSize(matrixSize),
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

QSize LedMatrix::size() const
{
    if(_panelSize.isValid() && _matrixSize.isValid())
    {
        return QSize(_panelSize.width() * _matrixSize.width(), _panelSize.height() * _matrixSize.height());
    }
    return QSize();
}

bool LedMatrix::isConfigured() const
{
    return size().isValid();
}

void LedMatrix::prepareFramebuffer()
{
    if(isConfigured())
    {
        _framebuffer.resize(size().width()*size().height()*3);
    }
}

void LedMatrix::computeLookUpTable()
{
    if(isConfigured())
    {
        const unsigned int matrix_panel_width_in_pixels = _panelSize.width();
        const unsigned int matrix_panel_height_in_pixels = _panelSize.height();

        const unsigned int matrix_width_in_panel = _matrixSize.width();
        //const unsigned int matrix_height_in_panel = _matrixSize.height();

        const unsigned int matrix_width_in_pixels = size().width();
        const unsigned int matrix_height_in_pixels = size().height();

        _pixelsLUT.clear();
        _pixelsLUT.resize(matrix_height_in_pixels*matrix_width_in_pixels);

        for (unsigned int y=0;y<matrix_height_in_pixels;y++)
        {
            for (unsigned int x=0;x<matrix_width_in_pixels;x++) {
                const unsigned int x_panel_id = (y%2)
                        ?((matrix_panel_width_in_pixels-1)-(x%matrix_panel_width_in_pixels))
                       :(x%matrix_panel_width_in_pixels);
                const unsigned int y_panel_id = (y%matrix_panel_height_in_pixels);
                const unsigned int panel_id = ((y/matrix_panel_height_in_pixels)%2)
                        ?((matrix_width_in_panel-1)-(x/matrix_panel_width_in_pixels)) + ((y/matrix_panel_height_in_pixels) * matrix_width_in_panel)
                       :(x/matrix_panel_width_in_pixels) + ((y/matrix_panel_height_in_pixels) * matrix_width_in_panel);

                const unsigned int id = x_panel_id + (y_panel_id*matrix_panel_width_in_pixels) + (panel_id*matrix_panel_width_in_pixels*matrix_panel_height_in_pixels);
                const unsigned int i = x+(y*matrix_width_in_pixels);
                Q_ASSERT(i<(unsigned int)_pixelsLUT.size());
                _pixelsLUT.data()[i] = id;
            }
        }
    }
}

void LedMatrix::show(const QImage *image)
{
    const QSize size(this->size());
    if(size.isValid())
    {
        const unsigned int matrix_width_in_pixels = size.width();
        const unsigned int matrix_height_in_pixels = size.height();
        const QRgb *pixels = reinterpret_cast<const QRgb*>(image->bits());

        for (unsigned int y=0;y<matrix_height_in_pixels;y++)
        {
            for (unsigned int x=0;x<matrix_width_in_pixels;x++) {
                const unsigned int id = _pixelsLUT.data()[x+(y*size.width())];

                const unsigned int r_id = (id * 3) + 1;
                const unsigned int g_id = (id * 3) + 2;
                const unsigned int b_id = (id * 3) + 0;

                QRgb rgb = pixels[x+(y*matrix_width_in_pixels)];

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
}
