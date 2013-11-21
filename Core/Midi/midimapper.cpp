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

MidiControl* MidiMapper::addMidiControl(const int interface, const quint8 channel, const quint8 control)
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

MidiControl* MidiMapper::findMidiControl(const int interface, const quint8 channel, const quint8 control, const bool autocreate)
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

MinoTrigger* MidiMapper::findMinoTriggerFromNote(const int interface, const quint8 channel, const quint8 note) const
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
    return _hashMinoTriggerNotes.value(key, NULL);
}

MinoTrigger* MidiMapper::findMinoTriggerFromControl(const int interface, const quint8 channel, const quint8 control) const
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
    return _hashMinoTriggerControls.value(key, NULL);
}

MinoControl* MidiMapper::findMinoControl(const int interface, const quint8 channel, const quint8 control) const
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
        /*
        qDebug() << Q_FUNC_INFO
                 << "no associated trigger found for note:" << note
                 << QString("(%1,%2,%3)").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
                 */
    }
}

void MidiMapper::mapNoteToRole(const int interface, const quint8 channel, const quint8 note, const QString &role)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
    MinoRole *mr = minoRoles().value(role, NULL);
    Q_ASSERT(mr);

    Q_ASSERT((mr->type() == MinoRole::Trigger) || (mr->type() == MinoRole::Hold));
    MinoTrigger *mt = minoTriggers().value(role, NULL);
    _hashMinoTriggerNotes.insert(key, mt);
}

void MidiMapper::mapControlToRole(const int interface, const quint8 channel, const quint8 control, const QString &role)
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
        Q_ASSERT(mt);
        qDebug() << Q_FUNC_INFO
                  << mt->role() << mt << " feedback(bool) is connected to triggerFeedback(bool)";
        connect(mt, SIGNAL(feedback(bool)), this, SLOT(triggerFeedback(bool)), Qt::UniqueConnection);

        _hashMinoTriggerControls.insert(key, mt);

        // This HACK allow MidiMapper to receive last feedback (again) in order to propagate feedback status to newly mapped control
        mt->forceFeedbackEmitting();

    }
        break;
    }
}

void MidiMapper::triggerFeedback(bool on)
{
    // FIXME this function is not generic and have been written with only nanoKontrol 2 LEDs usage in mind.
    MinoTrigger *mt = qobject_cast<MinoTrigger*>(sender());
    Q_ASSERT(mt);
    QStringList keys = _hashMinoTriggerControls.keys(mt);
    foreach(QString key, keys)
    {
        QStringList sl = key.split(':');
        Q_ASSERT(sl.count() == 3);
        const int id = sl.at(0).toInt();
        const int channel = sl.at(1).toInt();
        const int control = sl.at(2).toInt();
        MidiInterface *mi = Minotor::minotor()->midi()->findMidiInterface(id);
        // HACK to drive LEDs on nanoKontrol 2 (when LED mode have been setup as external using Korg software)
        mi->sendControlChange(channel, control, on?127:0);
    }
}

bool MidiMapper::registerTrigger(const QString &role, const QString &description, const MinoRole::Type type)
{
    Q_ASSERT((type == MinoRole::Trigger) || (type == MinoRole::Hold));
    registerRole(role, description, type);
    return connectTrigger(role, NULL, NULL, NULL, NULL, (type==MinoRole::Hold));
}

bool MidiMapper::registerTrigger(const QString &role, const QString &description, const QObject *receiver, const char *method, const QObject *sender, const char *signal, MinoRole::Type type, bool overwrite)
{
    Q_ASSERT((type == MinoRole::Trigger) || (type == MinoRole::Hold));
    if(!(overwrite && minoRoles().contains(role)))
        registerRole(role, description, type);
    return connectTrigger(role, receiver, method, sender, signal, (type == MinoRole::Hold), overwrite);
}

MinoTrigger* MidiMapper::connectTrigger(const QString &role, const QObject *receiver, const char *method, const QObject *sender, const char *signal, bool toggle, bool overwrite)
{
    MinoTrigger *trigger = minoTriggers().value(role, NULL);
    if(!overwrite)
    {
        if(trigger)
        {
            qDebug() << Q_FUNC_INFO
                     << "role:" << role << "is already registered!";
            Q_ASSERT(false);
            return false;
        }
    }
    else
    {
        if(trigger)
        {
            // Warning: do not disconnect feedback(bool) signal
            trigger->disconnect(SIGNAL(triggered()));
            trigger->disconnect(SIGNAL(toggled(bool)));
            trigger->disconnectFeedback();
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
        if(toggle)
            connect(trigger, SIGNAL(toggled(bool)), receiver, method);
        else
            connect(trigger, SIGNAL(triggered()), receiver, method);

        trigger->connectFeedback(sender, signal);
    }
    return trigger;
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

bool minoRoleLessThan(MinoRole *r1, MinoRole *r2)
 {
    return r1->name() < r2->name();
}

QList<MinoRole *> MidiMapper::registeredRoles()
{
    QHash<QString, MinoRole*>::const_iterator i = minoRoles().constBegin();
    QList<MinoRole *> roles;
    while (i != minoRoles().constEnd()) {
        roles.append(i.value());
        ++i;
    }

    qSort(roles.begin(), roles.end(), minoRoleLessThan);

    return roles;
}

QString MidiMapper::findMinoControlFromRole(const QString &role) const
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

QString MidiMapper::findMinoTriggerControlFromRole(const QString &role) const
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

void MidiMapper::flushMidiMapping(MidiInterface *mi)
{
    int deletedControlCount = 0;
    // Trigger controls
    QHash<QString, MinoTrigger*>::const_iterator itc = _hashMinoTriggerControls.constBegin();
    QStringList toBeDeleted;
    while (itc != _hashMinoTriggerControls.constEnd()) {
        QRegExp rx("(\\d+):(\\d+):(\\d+)");
        if(rx.indexIn(itc.key()) == -1)
        {
            qDebug() << Q_FUNC_INFO
                     << "no match for control:" << itc.key();
            Q_ASSERT(false);
        }
        else
        {
            QStringList sl = rx.capturedTexts();
            Q_ASSERT(sl.count()==4);
            if(sl.at(1).toInt() == mi->id())
            {
                toBeDeleted.append(itc.key());
                ++deletedControlCount;
            }
        }
        ++itc;
    }

    foreach(const QString& tbd, toBeDeleted)
    {
        _hashMinoTriggerControls.remove(tbd);
    }

    // Trigger note
    QHash<QString, MinoTrigger*>::const_iterator itn = _hashMinoTriggerNotes.constBegin();
    while (itn != _hashMinoTriggerNotes.constEnd()) {
        QRegExp rx("(\\d+):(\\d+):(\\d+)");
        if(rx.indexIn(itn.key()) == -1)
        {
            qDebug() << Q_FUNC_INFO
                     << "no match for control:" << itn.key();
            Q_ASSERT(false);
        }
        else
        {
            QStringList sl = rx.capturedTexts();
            Q_ASSERT(sl.count()==4);
            if(sl.at(1).toInt() == mi->id())
            {
                toBeDeleted.append(itn.key());
                ++deletedControlCount;
            }
        }
        ++itn;
    }

    foreach(const QString& tbd, toBeDeleted)
    {
        _hashMinoTriggerNotes.remove(tbd);
    }

    // Control Change (direct)
    QHash<QString, MinoControl*>::const_iterator icc = _hashMinoControls.constBegin();
    while (icc != _hashMinoControls.constEnd()) {
        QRegExp rx("(\\d+):(\\d+):(\\d+)");
        if(rx.indexIn(icc.key()) == -1)
        {
            qDebug() << Q_FUNC_INFO
                     << "no match for control:" << icc.key();
            Q_ASSERT(false);
        }
        else
        {
            QStringList sl = rx.capturedTexts();
            Q_ASSERT(sl.count()==4);
            if(sl.at(1).toInt() == mi->id())
            {
                toBeDeleted.append(icc.key());
                ++deletedControlCount;
            }
        }
        ++icc;
    }

    foreach(const QString& tbd, toBeDeleted)
    {
        _hashMinoControls.remove(tbd);
    }

    if(deletedControlCount)
    {
        qDebug() << Q_FUNC_INFO
                 << deletedControlCount << "controls/notes have been deleted";
    }
}

void MidiMapper::loadMidiMapping(MidiInterface *mi, MidiMapping * mm)
{
    flushMidiMapping(mi);
    const uint controlCount = _hashMinoTriggerNotes.count() + _hashMinoTriggerControls.count() + _hashMinoControls.count();
    qDebug() << Q_FUNC_INFO
             << controlCount << "controls/notes are available";

    QMapIterator<QString, QVariant> i(mm->map());

    while (i.hasNext()) {
        i.next();
        const QString role = i.key();
        if(!role.isEmpty() && role != "none") {
            const QStringList mappedTo = i.value().toStringList();
            Q_ASSERT(mappedTo.count()==3);
            const uint type = mappedTo.at(0).toUInt();
            switch (type) {
            case 0: // CC
            {
                const uint channel = mappedTo.at(1).toUInt();
                const uint control = mappedTo.at(2).toUInt();
                mapControlToRole(mi->id(), channel, control, role);
            }
                break;
            case 1: // Note
            {
                const uint channel = mappedTo.at(1).toUInt();
                const uint note = mappedTo.at(2).toUInt();
                mapNoteToRole(mi->id(), channel, note, role);
            }
                break;
            default:
                qDebug() << Q_FUNC_INFO
                         << "You should not be there...";
                Q_ASSERT(false);
            }
        }
    }

    const uint afterLoadControlCount = _hashMinoTriggerNotes.count() + _hashMinoTriggerControls.count() + _hashMinoControls.count();

    qDebug() << Q_FUNC_INFO
             << afterLoadControlCount - controlCount << "controls/notes have been loaded";

    mi->setAcceptClock(mm->acceptClock());
    mi->setAcceptProgramChange(mm->acceptProgramChange());

    mi->setAcceptControlChange(mm->acceptControlChange());
    mi->setAcceptNoteChange(mm->acceptNoteChange());
}
