#include "minoprogrambank.h"
#include "minotor.h"
#include <QDebug>

MinoProgramBank::MinoProgramBank(QObject *parent) :
    MinoPersistentObject(parent)
{

}


void MinoProgramBank::addProgram(MinoProgram *program)
{
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
}
