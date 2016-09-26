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

#include "midimapping.h"
#include "midimapper.h"

#include <QSettings>
#include <QDebug>
#include <QStringList>

#include <QMetaProperty>
#include <QMetaObject>

MidiMapping::MidiMapping(QObject *parent) :
    QObject(parent),
    _modified(false),
    _tracks(0),
    _knobsPerTrack(0),
    _triggersPerTrack(0)
{
}

void MidiMapping::setVendor(const QString &vendor)
{
    if(_vendor != vendor)
    {
        _vendor = vendor;
        _modified = true;
    }
}

void MidiMapping::setProduct(const QString &product)
{
    if(_product != product)
    {
        _product = product;
        _modified = true;
    }
}

void MidiMapping::setComment(const QString &comment)
{
    if(_comment != comment)
    {
        _comment = comment;
        _modified = true;
    }
}

void MidiMapping::setTracks(const int tracks)
{
    if(_tracks != tracks)
    {
        _tracks = tracks;
        _modified = true;
    }
}

void MidiMapping::setKnobsPerTrack(const int knobsPerTrack)
{
    if(_knobsPerTrack != knobsPerTrack)
    {
        _knobsPerTrack = knobsPerTrack;
        _modified = true;
    }
}

void MidiMapping::setTriggersPerTrack(const int triggersPerTrack)
{
    if(_triggersPerTrack != triggersPerTrack)
    {
        _triggersPerTrack = triggersPerTrack;
        _modified = true;
    }
}

void MidiMapping::addAssignedMidiControl(const QString &role, const quint8 channel, const quint8 control)
{
    qDebug() << Q_FUNC_INFO
             << "role" << role
             << "channel" << channel
             << "control" << control;
    QStringList sl;
    const uint type = 0; // Control Change
    sl << QString::number(type) << QString::number(channel) << QString::number(control);
    _map.insertMulti(role, sl);
    _modified = true;
}

MidiMapping *MidiMapping::loadFromFile(const QString& filename)
{
    QSettings mapping(filename, QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        MidiMapping *mm = new MidiMapping();
        // Restore properties values
        mapping.beginGroup("properties");
        QStringList properties = mapping.childKeys();
        foreach(const QString& key, properties)
        {
            int index = mm->metaObject()->indexOfProperty(key.toLatin1());
            if(index != -1)
            {
                QMetaProperty omp = mm->metaObject()->property(index) ;
                omp.write(mm, mapping.value(key));
//                qDebug() << Q_FUNC_INFO
//                         << "write property:" << key
//                         << "with value:" << mapping.value(key)
//                         << "on mm:" << mm;
            }
        }
        mapping.endGroup();
        return mm;
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "unable to parse file:" << filename;
    }
    return NULL;
}

void MidiMapping::saveToFile(MidiMapping *mm, const QString &filename)
{
    QSettings mapping(filename, QSettings::IniFormat);

    // Remove all entries in this group
    mapping.remove("");

    // Start an array of properties
    mapping.beginGroup("properties");
    mapping.remove("");

    int index = 0;
    for(int j=0; j<mm->metaObject()->propertyCount(); j++)
    {
        QMetaProperty omp = mm->metaObject()->property(j);
        // Skip the already-handled objectName property
        if(QString(omp.name()) != QString("objectName"))
        {
            mapping.setValue(omp.name(), omp.read(mm));
//            qDebug() << QString(" ").repeated(3)
//                     << omp.typeName()
//                     << omp.name()
//                     << omp.read(mm)
//                     << omp.isStored();
            ++index;
        }
    }
    // End of properties array
    mapping.endGroup();
    mapping.sync();
}
