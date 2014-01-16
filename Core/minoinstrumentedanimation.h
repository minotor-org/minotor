#ifndef MINOINSTRUMENTEDANIMATION_H
#define MINOINSTRUMENTEDANIMATION_H

#include "minoanimation.h"

#include "minopropertymidichannel.h"

#include <QList>

class MinoInstrumentNoteEvent
{
public:
    explicit MinoInstrumentNoteEvent(quint8 note, bool on, quint8 value):
            _note(note),
            _on(on),
            _value(value) {}

    quint8 note() const { return _note; }
    bool on() const { return _on; }
    quint8 value() const { return _value; }

private:
    quint8 _note;
    bool _on;
    quint8 _value;

};

class MinoInstrumentedAnimation : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoInstrumentedAnimation(QObject *parent);

    static QColor noteToColor(const quint8 note);
    unsigned int noteToPosX(const quint8 note, const unsigned int reservedSize=0);
    unsigned int noteToPosY(const quint8 note, const unsigned int reservedSize=0);
    void createItem();

    bool isAlive() const { return _alive; }

signals:
    
public slots:
    void handleNoteChange(int interface, quint8 channel, quint8 note, bool on, quint8 value);

protected:
    // Status
    bool _alive;
    virtual void setAlive(const bool on);

    // MIDI Channel property
    MinoPropertyMidiChannel *_midiChannel;

    // Note events: item creations
    virtual void _startNote(const uint uppqn, const quint8 note, const quint8 value) = 0;
    virtual void _stopNote(const uint uppqn, const quint8 note) { (void)note; (void)uppqn; }
    virtual void _processPendingNote(const uint uppqn, const quint8 note) { (void)note; (void)uppqn; }
    void processNotesEvents(const uint uppqn);

    // Item creation (ie. by user interface)
    int _itemCreationRequested;
    void processItemCreation(const uint uppqn);
    virtual void _createItem(const uint uppqn) = 0;

    QList<MinoInstrumentNoteEvent> _noteEvents;
    QList<int> _pendingNotes;
};

#endif // MINOINSTRUMENTEDANIMATION_H
