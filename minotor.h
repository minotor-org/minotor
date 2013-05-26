#ifndef MINOTOR_H
#define MINOTOR_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>
#include <QSettings>

#include "ledmatrix.h"
#include "midi.h"
#include "midimapper.h"

#include "minomaster.h"
#include "minoclocksource.h"
#include "minoprogrambank.h"

class MinoAnimation;

class Minotor : public QObject
{
    Q_OBJECT
public:
    explicit Minotor(QObject *parent = 0);
    ~Minotor();

    // Scene accessor
    QGraphicsScene *scene() { return &_scene; }

    // Channel accessors
    MinoMaster *master() { return _master; }
//    MinoCue *cue() { return _cue; }

    // LedMatrix
    LedMatrix *ledMatrix() { return _ledMatrix; }

    // MIDI Interfaces
    Midi *midi() { return _midi; }
    // MIDI mapping
    MidiMapper *midiMapper() { return _midiMapper; }

    // Clock source
    MinoClockSource *clockSource() { return _clockSource; }

    // Display rect
    const QRect displayRect() const { return _ledMatrix->rect(); }

    // Singleton accessor
    static Minotor *minotor() { static Minotor *minotor = new Minotor(); return minotor; }

    // Persistence
    void save(MinoPersistentObject* object, QSettings* parser);
    void load(QSettings* parser);

    //Program Bank
    MinoProgramBank* programBank() { return _programBank; }
    void changeProgramBank(MinoProgramBank *bank);
    void clearPrograms();

    // Debug
    void initWithDebugSetup();

signals:
    void beatToggled(bool active);
    void programBankChanged(QObject *bank);

public slots:
    // Clock handler
    void dispatchClock(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    // Midi messages handlers
    void handleMidiInterfaceProgramChange(int interface, quint8 channel, quint8 program);

private:
    // Scene
    QGraphicsScene _scene;

    // Master
    MinoMaster *_master;

    // External connections
    LedMatrix *_ledMatrix;

    // Midi interfaces
    Midi *_midi;

    // Midi mapper
    MidiMapper *_midiMapper;

    // Clock source (internal generator and Midi)
    MinoClockSource *_clockSource;

    void loadObject(QSettings* parser, const QString &className, QObject *parent);
    void loadObjects(QSettings *parser, QObject *parent);

    QObject *findParentFor(const QString& className);

    MinoProgramBank *_programBank;
};

#endif // MINOTOR_H
