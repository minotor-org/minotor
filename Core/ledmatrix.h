/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * Copyright 2015, 2017 Michiel Brink
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

#include <QVarLengthArray>

#include "artdmx.h"

#include "qextserialport.h"

class LedMatrix : public QObject
{
     Q_OBJECT
public:
    explicit LedMatrix(const QSize size, const QSize panelSize, const QSize matrixSize, QObject *parent = 0);
    ~LedMatrix();

    bool openPortByName(const QString &portName);
    bool openArtByIp(const QString &artIp);
    void closePort();
    QString portName() const;

    bool isConnected();

    void show(const QImage *image);

    // Accessors
    // Returns matrix's size in pixels
    QSize size() const;
    QRect rect() const { return QRect(QPoint(0,0), size()); }

    // Returns panel's size in pixels
    QSize panelSize() const { return _panelSize; }
    // Returns matrix's size in panels
    QSize matrixSize() const { return _matrixSize; }

private:
    QSize _panelSize;
    QSize _matrixSize;

    // Connection
    QextSerialPort *_port;
    bool _connected_tty;
    bool _connected_art;

    //artnet
    char* dmx_universe;
    std::vector<int> dmx_universe_temp;
    char* _ipAdress;

    // Returns true if LedMatrix is fully configured (ie. does have all requiered sizes sets)
    bool isConfigured() const;
    void prepareFramebuffer();
    QByteArray _framebuffer;

    void computeLookUpTable();
    QVarLengthArray<unsigned int> _pixelsLUT;


signals:
    void updated();
    void connected(bool connected = true);

public slots:
    
};

#endif // LEDMATRIX_H
