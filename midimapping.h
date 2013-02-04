#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include <QObject>

#include "midicontrol.h"
#include "minoproperty.h"

class MidiMapping : public QObject
{
    Q_OBJECT
public:
    explicit MidiMapping(QObject *parent = 0);
    
    void assignCapturedControlTo(MinoProperty *property);
    MidiControl* getMidiControlFor(MinoProperty *property);

protected:
    MidiControl* findMidiControl(int interface, quint8 channel, quint8 control, bool autocreate = true);

    bool _controlCaptureMode;
    MinoProperty* _currentControlCaptureMinoProperty;
    MidiControlList _midiControls;
signals:
    
public slots:
    void midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value);
};

#endif // MIDIMAPPING_H
