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
    MinoTrigger *trigger = minoTriggers().value(role, NULL);
    if(!trigger)
    {
        qDebug() << Q_FUNC_INFO
                 << "role:" << role << "is not registered!";
    }
    else
    {
        const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
        _hashMinoTriggerNotes.insert(key, trigger);
    }
}

void MidiMapper::mapControlToRole(int interface, quint8 channel, quint8 control, QString role)
{
    MinoTrigger *minoTrigger = minoTriggers().value(role, NULL);
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
    if(!minoTrigger)
    {
        MinoControl *minoControl = minoControls().value(role, NULL);
        if(!minoControl)
        {
            qDebug() << Q_FUNC_INFO
                     << "role:" << role << "is not registered!";
        }
        else
        {
            _hashMinoControls.insert(key, minoControl);
        }
    }
    else
    {
        _hashMinoTriggerControls.insert(key, minoTrigger);
    }
}

bool MidiMapper::registerTrigger(QString role, const QObject *receiver, const char *method, bool toogle, bool overwrite)
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

bool MidiMapper::registerControl(QString role, const QObject *receiver, const char *method, bool overwrite)
{
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

QStringList MidiMapper::registeredRoles()
{
    QStringList sl;
    sl.append(minoTriggers().keys());
    sl.append(minoControls().keys());
    return sl;
}
