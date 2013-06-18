#ifndef MIDIMAPPER_H
#define MIDIMAPPER_H

#include <QObject>
#include <QHash>

#include "midicontrol.h"
#include "minotrigger.h"
#include "minocontrol.h"

#include "midicontrollableparameter.h"

class Minotor;

class MinoRole
{
public:
    enum Type {
        Direct,     // Associated to CC, MinoControl will use directly value received value
        Trigger,    // Associated to note/CC, a function will be triggered on each press (eg. play(), stop())
        Hold        // Associated to note/CC, a function will be called on each status change (eg. setEnable(bool))
    };

    explicit MinoRole(const QString &role, const QString &description, MinoRole::Type type):
        _name(role),
        _description(description),
        _type(type) { }

    QString name() const { return _name; }
    QString description() const { return _description; }
    MinoRole::Type type() const { return _type; }

private:
    QString _name;
    QString _description;
    MinoRole::Type _type;
};

class MidiMapper : public QObject
{
    Q_OBJECT
public:
    explicit MidiMapper(Minotor *minotor);
    ~MidiMapper();
    
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

    // Register a new role with trigger attibutes
    static bool registerTrigger(const QString &role, const QString &description, const MinoRole::Type type);
    // Register and connect a new role with trigger attibutes
    static bool registerTrigger(const QString &role, const QString &description, const QObject *receiver = NULL, const char *method = NULL, MinoRole::Type type = MinoRole::Trigger, bool overwrite = false);
    // Connect an already-registered trigger to slot
    static bool connectTrigger(const QString &role, const QObject *receiver, const char *method, bool toogle = false, bool overwrite = false);

    // Register and connect a new role
    static bool registerControl(const QString &role, const QString &description, const QObject *receiver = NULL, const char *method = NULL, bool overwrite = false);
    // Connect an already-registered control to slot
    static bool connectControl(const QString &role, const QObject *receiver, const char *method, bool overwrite = false);
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

    // Registered roles
    QHash<QString, MinoRole*> _hashMinoRoles;

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

    static QHash<QString, MinoRole*>& minoRoles()
    {
        static QHash<QString, MinoRole*> roles;
        return roles;
    }
    static void registerRole(const QString &name, const QString &description, MinoRole::Type type);

signals:
    
private slots:
    void midiControlChanged(int interface, quint8 channel, quint8 control, quint8 value);
    void noteChanged(int interface, quint8 channel, quint8 note, bool on, quint8 value);
};

#endif // MIDIMAPPER_H
