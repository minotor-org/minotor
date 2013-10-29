/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MINOTOR_H
#define MINOTOR_H

#include <QObject>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QPropertyAnimation>
#include <QSettings>
#include <QPixmap>

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
    const QRect displayRect() const { return QRect(QPoint(0,0), _rendererSize); }
    const QSize displaySize() const { return _rendererSize; }

    // Singleton accessor
    static Minotor *minotor() { static Minotor *minotor = new Minotor(); return minotor; }

    // Persistence
    void save(MinoPersistentObject* object, QSettings* parser);
    void load(QSettings* parser);

    //Program Bank
    MinoProgramBank* programBank() { return _programBank; }
    void changeProgramBank(MinoProgramBank *bank);
    void clearPrograms();

    // Screenshots
    QPixmap* graphicsItemToPixmap(QGraphicsItem *item);

    // Debug
    void initWithDebugSetup();

    // Settings
    QSettings* settings() const { return _settings; }

    void loadSettings();
    void saveSettings();

signals:
    void beatToggled(bool active);
    void programBankChanged(QObject *bank);

public slots:
    // Clock handler
    void dispatchClock(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    // Midi messages handlers
    void handleMidiInterfaceProgramChange(int interface, quint8 channel, quint8 program);

private:
    // Settings
    QSettings *_settings;
    void loadMidiSettings();
    void loadLedMatrixSettings();

    // Scene
    QGraphicsScene _scene;
    QSize _rendererSize;

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
