#include "midimapping.h"
#include "midimapper.h"

#include <QSettings>
#include <QDebug>

MidiMapping::MidiMapping(QObject *parent) :
    QObject(parent),
    _modified(false)
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

void MidiMapping::addAssignedMidiControl(const QString &role, const quint8 channel, const quint8 control)
{
    qDebug() << Q_FUNC_INFO
             << "role" << role
             << "channel" << channel
             << "control" << control;
    _map.insertMulti(role, QString("CC%1:%2").arg(QString::number(channel)).arg(QString::number(control)));
    _modified = true;
}

MidiMapping *MidiMapping::loadFromFile(const QString& filename, QObject *parent)
{
    QSettings mapping(filename, QSettings::IniFormat);
    if(QSettings::NoError == mapping.status())
    {
        MidiMapping *mm = new MidiMapping(parent);
        // General
        mapping.beginGroup("general");
        const QString vendor = mapping.value("vendor", QString("undefined")).toString();
        const QString product = mapping.value("product", QString("undefined")).toString();
        mm->setVendor(vendor);
        mm->setProduct(product);
        mapping.endGroup();

        // MIDI Controls
        int size = mapping.beginReadArray("midi_controls");
        for(int i = 0; i < size; ++i)
        {
            mapping.setArrayIndex(i);
            mm->addAssignedMidiControl(mapping.value("role").toString(), mapping.value("channel").toUInt(), mapping.value("control").toUInt());
        }
        mapping.endArray();
        return mm;
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "unable to parse file:" << filename;
    }
    return NULL;
}
