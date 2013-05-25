#include "minomaster.h"

#include "minotor.h"
#include "minoanimationgroup.h"
#include "minopropertyreal.h"

#include <QDebug>

MinoMaster::MinoMaster(Minotor *minotor):
    QObject(),
    _program(NULL),
    _shifted(false)
{
    minotor->scene()->addItem(&_itemGroup);

    MinoPropertyReal *mpBrightness = new MinoPropertyReal(this);
    mpBrightness->setValue(1.0);
    mpBrightness->setObjectName("master-brightness");
    mpBrightness->setLabel("Brightness");
    connect(mpBrightness, SIGNAL(valueChanged(qreal)), this, SLOT(setBrightness(qreal)));

    _midiMapper = new MinoMasterMidiMapper(this);
}

MinoMaster::~MinoMaster()
{
    if(_program)
    {
        disconnect(_program, SIGNAL(updated()), this, SIGNAL(updated()));
        _itemGroup.removeFromGroup(_program->itemGroup());
    }
}

void MinoMaster::setBrightness(qreal value)
{
    _itemGroup.setOpacity(value);
}

void MinoMaster::setProgram(MinoProgram *program)
{
    if (_program != program)
    {
        if(_program)
        {
            disconnect(_program, SIGNAL(updated()), this, SIGNAL(updated()));
            disconnect(_program, SIGNAL(destroyed()), this, SLOT(clear()));
            _itemGroup.removeFromGroup(_program->itemGroup());
            _program->setOnAir(false);
        }
        if(program)
        {
            connect(program, SIGNAL(updated()), this, SIGNAL(updated()));
            connect(program,SIGNAL(destroyed()), this, SLOT(clear()));
            _itemGroup.addToGroup(program->itemGroup());
	        program->setOnAir(true);
        }
        _program = program;
        emit programChanged();
    }
}

void MinoMaster::setViewportRange(const int min, const int max)
{
    emit viewportRangeChanged(min,max);
}

void MinoMaster::clear()
{
    _program = NULL;
    emit programChanged();
}
