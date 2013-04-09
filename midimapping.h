#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include <QObject>
#include <QHash>

#include "midicontrol.h"
#include "minotrigger.h"
#include "minocontrol.h"

#include "midicontrollableparameter.h"

class Minotor;

class MidiMapping : public QObject
{
    Q_OBJECT
public:
    explicit MidiMapping(Minotor *minotor = 0);
    
    void assignCapturedControlTo(MidiControllableParameter *parameter);
    MidiControl* getMidiControlFor(MidiControllableParameter *parameter);

    MidiControlList midiControls() const { return _midiControls; }

    MidiControl* addMidiControl(int interface, quint8 channel, quint8 control);
    MidiControl* findMidiControl(int interface, quint8 channel, quint8 control, bool autocreate);

    MinoTrigger* findMinoTriggerFromNote(int interface, quint8 channel, quint8 note);
    MinoTrigger* findMinoTriggerFromControl(int interface, quint8 channel, quint8 control);
    MinoControl* findMinoControl(int interface, quint8 channel, quint8 control);

    void mapNoteToRole(int interface, quint8 channel, quint8 note, QString role);
    void mapControlToRole(int interface, quint8 channel, quint8 control, QString role);

    static bool registerTrigger(QString role, const QObject *receiver = NULL, const char *method = NULL, bool toogle = false, bool overwrite = false);
    static bool registerControl(QString role, const QObject *receiver = NULL, const char *method = NULL, bool overwrite = false);
    static QStringList registeredRoles();

protected:
    bool _controlCaptureMode;
    MidiControllableParameter * _currentControlCaptureParameter;
    MidiControlList _midiControls;

    // Notes -> MinoTrigger* association
    QHash<QString, MinoTrigger*> _hashMinoTriggerNotes;

    // Controls -> MinoTrigger* association
    QHash<QString, MinoTrigger*> _hashMinoTriggerControls;

    // Controls -> MinoControl* association
    QHash<QString, MinoControl*> _hashMinoControls;

private:
    static QHash<QString, MinoTrigger*>& minoTriggers()
    {
        static QHash<QString, MinoTrigger*> triggers;
        return triggers;
    }

    static QHash<QString, MinoControl*>& minoControls()
    {
        static QHash<QString, MinoControl*> controls;
        return controls;
    }

signals:
    
private slots:
    void midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value);
    void noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value);
};

#endif // MIDIMAPPING_H
