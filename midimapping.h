#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include <QObject>

#include "midicontrol.h"
#include "minoanimationproperty.h"

class MidiMapping : public QObject
{
    Q_OBJECT
public:
    explicit MidiMapping(QObject *parent = 0);
    
    void assignCapturedControlTo(MinoAnimationProperty *property);
    MidiControl* getMidiControlFor(MinoAnimationProperty *property);

protected:
    MidiControl* findMidiControl(int interface, quint8 channel, quint8 control, bool autocreate = true);

    bool _controlCaptureMode;
    MinoAnimationProperty* _currentControlCaptureMinoAnimationProperty;
    MidiControlList _midiControls;
signals:
    
public slots:
    void midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value);
};

#endif // MIDIMAPPING_H
