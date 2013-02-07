#include "minomaster.h"

#include "minotor.h"

#include <QDebug>

MinoMaster::MinoMaster():
    QObject(),
    _program(NULL),
    _shifted(false)
{
}

MinoMaster::~MinoMaster()
{

}

void MinoMaster::setBrightness(qreal value)
{
    _program->itemGroup()->setOpacity(value);
}

void MinoMaster::setProgram(MinoProgram *program)
{
    if (_program != program)
    {
        _program = program;
        emit programChanged();
    }
}


#define KORG_NOTE_SHIFT 24
#define KORG_NOTE_MIN   9
#define KORG_NOTE_MAX   24

void MinoMaster::noteChanged(quint8 channel, quint8 note, bool on, quint8 value)
{
    if(_program)
    {
        qDebug() << "note changed" << channel << note << on << value;
        switch(note)
        {
        case KORG_NOTE_SHIFT:
            _shifted = on;
            break;
        default:
        {
            const int noteMin = KORG_NOTE_MIN;
            const int noteMax = qMax(KORG_NOTE_MAX, noteMin + _program->animations().count() - 1);
            if ((note >= noteMin) && (note <= noteMax))
            {
                const int i = note-noteMin;
                qDebug() << "setEnabled" << on << i;
                if(_shifted)
                {
                    if (on)
                        _program->animations().at(i)->setDelayedEnabled(!_program->animations().at(i)->enabled());
                }else{
                    _program->animations().at(i)->setEnabled(on);
                }
            }
        }
        }
    }
}
