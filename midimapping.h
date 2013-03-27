#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include <QObject>

#include "midicontrol.h"
#include "minomidicontrolableproperty.h"

class Minotor;

class MidiMapping : public QObject
{
    Q_OBJECT
public:
    explicit MidiMapping(Minotor *minotor = 0);
    
    void assignCapturedControlTo(MinoMidiControlableProperty *property);
    MidiControl* getMidiControlFor(MinoMidiControlableProperty *property);

    MidiControlList midiControls() const { return _midiControls; }

    MidiControl* addMidiControl(int interface, quint8 channel, quint8 control);
    MidiControl* findMidiControl(int interface, quint8 channel, quint8 control, bool autocreate);

protected:
    bool _controlCaptureMode;
    MidiControlableProperty* _currentControlCaptureMinoProperty;
    MidiControlList _midiControls;

signals:
    
private slots:
    void midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value);
};

#endif // MIDIMAPPING_H
