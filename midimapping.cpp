#include "midimapping.h"

#include <QDebug>

MidiMapping::MidiMapping(QObject *parent) :
    QObject(parent),
    _controlCaptureMode(false),
    _currentControlCaptureMinoProperty(NULL)
{

}

void MidiMapping::assignCapturedControlTo(MinoProperty *property)
{
    if(property)
    {
        _currentControlCaptureMinoProperty = property;
        _controlCaptureMode = true;
    }
    else
    {
        _controlCaptureMode = false;
    }
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
        midiControl = new MidiControl(interface, channel, control, this);
        _midiControls.append(midiControl);
        qDebug() << "New mapped control:" << interface << channel << control;
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
