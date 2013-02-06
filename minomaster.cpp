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


void MinoMaster::noteChanged(quint8 channel, quint8 note, bool on, quint8 value)
{
    if(_program)
    {
        qDebug() << "note changed" << channel << note << on << value;
        switch(note)
        {
        case 72:
            _shifted = on;
            break;
        default:
        {
            const int noteMin = 57;
            const int noteMax = qMax(60, noteMin + _program->animations().count() - 1);
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
