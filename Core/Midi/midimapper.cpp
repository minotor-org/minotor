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

#include "midimapper.h"

#include "minotor.h"

#include <QDebug>

MidiMapper::MidiMapper(Minotor *minotor) :
    QObject(minotor),
    _controlCaptureMode(false),
    _currentControlCaptureParameter(NULL)
{
    // Link Minotor to MidiMapper
    connect(minotor->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
    connect(minotor->midi(), SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)), this, SLOT(noteChanged(int,quint8,quint8,bool,quint8)));
}

MidiMapper::~MidiMapper()
{
    foreach(MinoTrigger *t, minoTriggers())
        delete t;
    minoTriggers().clear();

    foreach(MinoControl *c, minoControls())
        delete c;
    minoControls().clear();

    foreach(MinoRole *r, minoRoles())
        delete r;
    minoRoles().clear();
}

void MidiMapper::assignCapturedControlTo(MidiControllableParameter *parameter)
{
    if(parameter)
    {
        _currentControlCaptureParameter = parameter;
        _controlCaptureMode = true;
    }
    else
    {
        _controlCaptureMode = false;
    }
}

MidiControl* MidiMapper::addMidiControl(int interface, quint8 channel, quint8 control)
{
    if (findMidiControl(interface, channel, control, false))
    {
        qDebug() << Q_FUNC_INFO
                 << "Mega big error \\o/";
        Q_ASSERT(false);
    }
    MidiControl *midiControl = new MidiControl(interface, channel, control, this);
    _midiControls.append(midiControl);
    qDebug() << Q_FUNC_INFO
             << "New mapped control:" << interface << channel << control;
    return midiControl;
}

MidiControl* MidiMapper::findMidiControl(int interface, quint8 channel, quint8 control, bool autocreate)
{
    MidiControl *midiControl = NULL;
    foreach (midiControl, _midiControls)
    {
        if ((midiControl->interface() == interface) &&
                (midiControl->channel() == channel) &&
                (midiControl->control() == control))
        {
            return midiControl;
        }
    }
    midiControl = NULL;

    // Requested MidiControl is not found
    if(autocreate)
    {
        midiControl = addMidiControl(interface, channel, control);
    }
    return midiControl;
}

void MidiMapper::midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value)
{
    qDebug() << Q_FUNC_INFO
             << "control changed:" << interface << channel << control << value;
    MidiControl *midiControl = findMidiControl(interface, channel, control, _controlCaptureMode);
    if(midiControl)
    {
        midiControl->setValue(value);

        if(_controlCaptureMode)
        {
            _controlCaptureMode = false;
            // FIXME
            // _currentControlCaptureMinoProperty->setMidiControl(midiControl);
            _currentControlCaptureParameter = NULL;
        }
    }
    MinoTrigger *minoTrigger = findMinoTriggerFromControl(interface, channel, control);
    if(minoTrigger)
    {
        minoTrigger->setStatus(value==127);
        qDebug() << Q_FUNC_INFO
                 << "trigger found:" << minoTrigger << minoTrigger->role();
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "no associated trigger found.";
        MinoControl *minoControl = findMinoControl(interface, channel, control);
        if(minoControl)
        {
            minoControl->setValue(value);
            qDebug() << Q_FUNC_INFO
                     << "control found:" << minoControl << minoControl->role();
        }
        else
        {
            qDebug() << Q_FUNC_INFO
                     << "no associated control found.";
        }
    }
}

MinoTrigger* MidiMapper::findMinoTriggerFromNote(int interface, quint8 channel, quint8 note)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
    return _hashMinoTriggerNotes.value(key, NULL);
}

MinoTrigger* MidiMapper::findMinoTriggerFromControl(int interface, quint8 channel, quint8 control)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
    return _hashMinoTriggerControls.value(key, NULL);
}

MinoControl* MidiMapper::findMinoControl(int interface, quint8 channel, quint8 control)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
    return _hashMinoControls.value(key, NULL);
}

void MidiMapper::noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value)
{
    MinoTrigger *minoTrigger = findMinoTriggerFromNote(interface, channel, note);
    if(minoTrigger)
    {
        // Some MIDI controllers (eg. BCD3000) do not send 'note off' but 'note on' with no velocity (value==0)
        if(value==0) on = false;
        minoTrigger->setStatus(on);
        qDebug() << Q_FUNC_INFO
                 << "trigger found:" << minoTrigger->role()
                 << "(" << on << ")";
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "no associated trigger found for note:" << note
                 << QString("(%1,%2,%3)").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
    }
}

void MidiMapper::mapNoteToRole(int interface, quint8 channel, quint8 note, QString role)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
    MinoRole *mr = minoRoles().value(role, NULL);
    Q_ASSERT(mr);

    Q_ASSERT((mr->type() == MinoRole::Trigger) || (mr->type() == MinoRole::Hold));
    MinoTrigger *mt = minoTriggers().value(role, NULL);
    _hashMinoTriggerNotes.insert(key, mt);
}

void MidiMapper::mapControlToRole(int interface, quint8 channel, quint8 control, QString role)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
    MinoRole *mr = minoRoles().value(role, NULL);
    Q_ASSERT(mr);

    switch(mr->type())
    {
    case MinoRole::Direct:
    {
        MinoControl *mc = minoControls().value(role, NULL);
        _hashMinoControls.insert(key, mc);
    }
        break;
    case MinoRole::Hold:
    case MinoRole::Trigger:
    {
        MinoTrigger *mt = minoTriggers().value(role, NULL);
        _hashMinoTriggerControls.insert(key, mt);
    }
        break;
    }
}

bool MidiMapper::registerTrigger(const QString &role, const QString &description, const MinoRole::Type type)
{
    Q_ASSERT((type == MinoRole::Trigger) || (type == MinoRole::Hold));
    registerRole(role, description, type);
    return connectTrigger(role, NULL, NULL, (type==MinoRole::Hold));
}

bool MidiMapper::registerTrigger(const QString &role, const QString &description, const QObject *receiver, const char *method, MinoRole::Type type, bool overwrite)
{
    Q_ASSERT((type == MinoRole::Trigger) || (type == MinoRole::Hold));
    if(!(overwrite && minoRoles().contains(role)))
        registerRole(role, description, type);
    return connectTrigger(role, receiver, method, (type == MinoRole::Hold), overwrite);
}

bool MidiMapper::connectTrigger(const QString &role, const QObject *receiver, const char *method, bool toogle, bool overwrite)
{
    MinoTrigger *trigger = minoTriggers().value(role, NULL);
    if(!overwrite)
    {
        if(trigger)
        {
            qDebug() << Q_FUNC_INFO
                     << "role:" << role << "is already registered!";
            return false;
        }
    }
    else
    {
        if(trigger)
        {
            trigger->disconnect();
        }
    }
    if(!trigger)
    {
        trigger = new MinoTrigger(role);
        minoTriggers().insert(role, trigger);
//        qDebug() << Q_FUNC_INFO
//                 << "role:" << role << "is now registered.";
    }
    if(receiver && method)
    {
        if(toogle)
            connect(trigger, SIGNAL(toogled(bool)), receiver, method);
        else
            connect(trigger, SIGNAL(triggered()), receiver, method);

//        qDebug() << Q_FUNC_INFO
//                 << "role:" << role << "is now connected to: " << receiver << QString("(%1)").arg(QString(method));
    }
    return true;
}

bool MidiMapper::registerControl(const QString &role, const QString &description, const QObject *receiver, const char *method, bool overwrite)
{
    if(!(overwrite && minoRoles().contains(role)))
        registerRole(role, description, MinoRole::Direct);

    return connectControl(role, receiver, method, overwrite);
}

bool MidiMapper::connectControl(const QString &role, const QObject *receiver, const char *method, bool overwrite)
{
    Q_ASSERT(minoRoles().contains(role));

    MinoControl *minoControl = minoControls().value(role, NULL);
    if(!overwrite)
    {
        if(minoControl)
        {
            qDebug() << Q_FUNC_INFO
                     << "role:" << role << "is already registered!";
            Q_ASSERT(!minoControl);
            return false;
        }
    }
    else
    {
        if(minoControl)
        {
            minoControl->disconnect();
        }
    }
    if(!minoControl)
    {
        minoControl = new MinoControl(role);
        minoControls().insert(role, minoControl);
//        qDebug() << Q_FUNC_INFO
//                 << "role:" << role << "is now registered.";
    }
    if(receiver && method)
    {
        connect(minoControl, SIGNAL(valueChanged(quint8)), receiver, method);
//        qDebug() << Q_FUNC_INFO
//                 << "role:" << role << "is now connected to: " << receiver << QString("(%1)").arg(QString(method));
    }
    return true;
}

void MidiMapper::registerRole(const QString &name, const QString &description, MinoRole::Type type)
{
    Q_ASSERT(!minoRoles().contains(name));
    minoRoles().insert(name, new MinoRole(name, description, type));
}

QList<MinoRole *> MidiMapper::registeredRoles()
{
    QHash<QString, MinoRole*>::const_iterator i = minoRoles().constBegin();
    QList<MinoRole *> roles;
    while (i != minoRoles().constEnd()) {
        roles.append(i.value());
        ++i;
    }
    return roles;
}

QString MidiMapper::findMinoControlFromRole(const QString &role)
{
    QHash<QString, MinoControl*>::const_iterator i = _hashMinoControls.constBegin();
    while (i != _hashMinoControls.constEnd()) {
        MinoControl *mc = i.value();
        if(mc->role() == role)
        {
            return i.key();
        }
        ++i;
    }
    return "";
}

QString MidiMapper::findMinoTriggerControlFromRole(const QString &role)
{
    QHash<QString, MinoTrigger*>::const_iterator i = _hashMinoTriggerControls.constBegin();
    while (i != _hashMinoTriggerControls.constEnd()) {
        MinoTrigger *mt = i.value();
        if(mt->role() == role)
        {
            return i.key();
        }
        ++i;
    }
    return "";
}
