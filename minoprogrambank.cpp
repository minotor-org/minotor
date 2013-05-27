#include "minoprogrambank.h"
#include "minotor.h"
#include <QDebug>

MinoProgramBank::MinoProgramBank(QObject *parent) :
    MinoPersistentObject(parent),
    _programSelectorPos(-1)
{
    MidiMapper::registerTrigger("BANK_NEXT", this, SLOT(programSelectorNext()), false, true);
    MidiMapper::registerTrigger("BANK_PREVIOUS", this, SLOT(programSelectorPrevious()), false, true);
    MidiMapper::registerTrigger("BANK_SELECT", this, SLOT(programSelectorSelect()), false, true);
}


void MinoProgramBank::addProgram(MinoProgram *program)
{
    _programSelectorPos = -1;
    emit programSelectorChanged(NULL);

    _programs.append(program);
    const int id = _programs.indexOf(program);

    // Program ID starts at 1
    program->setId(id+1);

    LedMatrix *matrix = minotor()->ledMatrix();
    // Inform program about matrix size (used by animations)
    program->setRect(matrix->rect());
    // Drawing rect
    // On the scene, the program have a dedicated area to display/draw animations
    // This area left one "screen" before and one "screen" areas to prevent from collisions
    // Note: Developer of animations should take care to not collide: its objects should never be larger than one screen-size in all directions (up, down, left, right, diagonals)
    QPointF pos = QPointF(matrix->size().width()*3, matrix->size().height() + ((matrix->size().height()*3) * id));
    program->setDrawingPos(pos);
    connect(program,SIGNAL(destroyed(QObject*)),this,SLOT(destroyProgram(QObject*)));
    emit programAdded(program);
}

MinoProgramBank::~MinoProgramBank()
{
    foreach(MinoProgram *prg, _programs)
    {
        delete prg;
    }
    _programs.clear();
}

Minotor* MinoProgramBank::minotor()
{
    Minotor* minotor =  qobject_cast<Minotor*>(parent());
    Q_ASSERT(minotor);
    return minotor;
}

void MinoProgramBank::destroyProgram(QObject *program)
{    
    _programs.removeAt(_programs.indexOf(static_cast<MinoProgram*>(program)));
    _programSelectorPos = -1;
    emit programSelectorChanged(NULL);
}

void MinoProgramBank::programSelectorNext()
{
    if (++_programSelectorPos >= _programs.count() )
    {
        _programSelectorPos = 0;
    }
    Q_ASSERT(_programs.at(_programSelectorPos));
    emit programSelectorChanged(_programs.at(_programSelectorPos));
}

void MinoProgramBank::programSelectorPrevious()
{
    if (--_programSelectorPos < 0)
    {
        _programSelectorPos = _programs.count() - 1;
    }
    Q_ASSERT(_programs.at(_programSelectorPos));
    emit programSelectorChanged(_programs.at(_programSelectorPos));
}

void MinoProgramBank::programSelectorSelect()
{
    if((_programSelectorPos >= 0)
            && (_programSelectorPos < _programs.count()))
    {
        Minotor::minotor()->master()->setProgram(_programs.at(_programSelectorPos));
    }
}
