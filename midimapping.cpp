#include "midimapping.h"

#include "minotor.h"

#include <QDebug>

MidiMapping::MidiMapping(Minotor *minotor) :
    QObject(minotor),
    _controlCaptureMode(false),
    _currentControlCaptureMinoProperty(NULL)
{
    // Link Minotor to MidiMapping
    connect(minotor->midi(), SIGNAL(controlChanged(int,quint8,quint8,quint8)), this, SLOT(midiControlChanged(int,quint8,quint8,quint8)));
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
                 << "Mega big error \o/";
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
}
