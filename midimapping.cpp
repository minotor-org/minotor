#include "midimapping.h"

#include "minotor.h"
#include "minotrigger.h"

#include <QDebug>

MidiMapping::MidiMapping(Minotor *minotor) :
    QObject(minotor),
    _controlCaptureMode(false),
    _currentControlCaptureMinoProperty(NULL)
{
    // Link Minotor to MidiMapping
    connect(minotor->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
    connect(minotor->midi(), SIGNAL(noteChanged(int,quint8,quint8,bool,quint8)), this, SLOT(noteChanged(int,quint8,quint8,bool,quint8)));
}

void MidiMapping::assignCapturedControlTo(MinoMidiControlableProperty *property)
{
    if(property)
    {
        _currentControlCaptureMinoProperty = property->_midiControlableProperty;
        _controlCaptureMode = true;
    }
    else
    {
        _controlCaptureMode = false;
    }
}

MidiControl* MidiMapping::addMidiControl(int interface, quint8 channel, quint8 control)
{
    if (findMidiControl(interface, channel, control, false))
    {
        qDebug() << Q_FUNC_INFO
                 << "Mega big error \\o/";
    }
    MidiControl *midiControl = new MidiControl(interface, channel, control, this);
    _midiControls.append(midiControl);
    qDebug() << Q_FUNC_INFO
             << "New mapped control:" << interface << channel << control;
    return midiControl;
}

MidiControl* MidiMapping::findMidiControl(int interface, quint8 channel, quint8 control, bool autocreate)
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

void MidiMapping::midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value)
{
    qDebug() << Q_FUNC_INFO
             << "control changed:" << interface << channel << control << value;
    MidiControl *midiControl = findMidiControl(interface, channel, control, _controlCaptureMode);
    if(midiControl)
    {
        qDebug() << "control" << control << "value" << value;
        midiControl->setValue(value);

        if(_controlCaptureMode)
        {
            _controlCaptureMode = false;
            _currentControlCaptureMinoProperty->setMidiControl(midiControl);
            _currentControlCaptureMinoProperty = NULL;
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
    }
}

MinoTrigger* MidiMapping::findMinoTriggerFromNote(int interface, quint8 channel, quint8 note)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(note));
    return _hashMinoTriggerNotes.value(key, NULL);
}

MinoTrigger* MidiMapping::findMinoTriggerFromControl(int interface, quint8 channel, quint8 control)
{
    const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
    return _hashMinoTriggerControls.value(key, NULL);
}

void MidiMapping::noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value)
{
    qDebug() << Q_FUNC_INFO
             << "note changed:" << interface << channel << note << on << value;
    MinoTrigger *minoTrigger = findMinoTriggerFromNote(interface, channel, note);
    if(minoTrigger)
    {
        minoTrigger->setStatus(on);
        qDebug() << Q_FUNC_INFO
                 << "trigger found:" << minoTrigger << minoTrigger->role();
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "no associated trigger found.";
    }
}

void MidiMapping::mapNoteToRole(int interface, quint8 channel, quint8 note, QString role)
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

void MidiMapping::mapControlToRole(int interface, quint8 channel, quint8 control, QString role)
{
    MinoTrigger *trigger = minoTriggers().value(role, NULL);
    if(!trigger)
    {
        qDebug() << Q_FUNC_INFO
                 << "role:" << role << "is not registered!";
    }
    else
    {
        const QString key = QString("%1:%2:%3").arg(QString::number(interface)).arg(QString::number(channel)).arg(QString::number(control));
        _hashMinoTriggerControls.insert(key, trigger);
    }
}

void MidiMapping::registerTrigger(QString role, const QObject *receiver, const char *method, bool toogle)
{
    if(minoTriggers().value(role, NULL))
    {
        qDebug() << Q_FUNC_INFO
                 << "role:" << role << "is already registered!";
    }
    else
    {
        MinoTrigger *trigger = new MinoTrigger(role);
        if(toogle)
            connect(trigger, SIGNAL(toogled(bool)), receiver, method);
        else
            connect(trigger, SIGNAL(triggered()), receiver, method);

        minoTriggers().insert(role, trigger);
        qDebug() << Q_FUNC_INFO
                 << "role:" << role << "is now connected to: " << receiver << QString("(%1)").arg(QString(method));
    }
}
