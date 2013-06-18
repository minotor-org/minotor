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

#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include <QObject>
#include <QMap>
#include <QVariant>

class MidiMapping : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString vendor READ vendor WRITE setVendor STORED true)
    Q_PROPERTY(QString product READ product WRITE setProduct STORED true)
    Q_PROPERTY(QMap map READ map STORED true)
public:
    explicit MidiMapping(QObject *parent = 0);
    
    void setVendor(const QString& vendor);
    QString vendor() const { return _vendor; }

    void setProduct(const QString& product);
    QString product() const { return _product; }

    QMap<QString,QVariant> map() const { return _map; }
    void addAssignedMidiControl(const QString &role, const quint8 channel, const quint8 control);

    static MidiMapping *loadFromFile(const QString &filename, QObject *parent = 0);
//    static bool saveToFile(QString filename);

signals:
    
public slots:

private:
    QString _vendor;
    QString _product;
    QMap<QString,QVariant> _map;
    bool _modified;
};

#endif // MIDIMAPPING_H
