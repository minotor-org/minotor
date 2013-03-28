#include "minomaster.h"

#include "minotor.h"
#include "minoanimationgroup.h"

#include <QDebug>

MinoMaster::MinoMaster(Minotor *minotor):
    QObject(),
    _program(NULL),
    _shifted(false)
{
    minotor->scene()->addItem(&_itemGroup);

    MinoPropertyReal *mpBrightness = new MinoPropertyReal();
    mpBrightness->setValue(1.0);
    mpBrightness->setObjectName("Brightness");
    connect(mpBrightness, SIGNAL(valueChanged(qreal)), this, SLOT(setBrightness(qreal)));
    _properties.append(mpBrightness);

    for (int i=0; i<100; i++)
    {
        QString role = QString("MASTER_ANIMATION_%1").arg(i);
        MidiMapping::registerTrigger(role);
    }
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
            _itemGroup.removeFromGroup(_program->itemGroup());
            _program->setOnAir(false);
        }
        if(program)
        {
            connect(program, SIGNAL(updated()), this, SIGNAL(updated()));
            _itemGroup.addToGroup(program->itemGroup());
	        program->setOnAir(true);

            for(int i=0; i<program->animationGroups().count(); ++i)
            {
                QString role = QString("MASTER_ANIMATION_%1").arg(i);
                MidiMapping::registerTrigger(role, program->animationGroups().at(i), SLOT(setEnabled(bool)), true, true);
            }
        }
        _program = program;
        emit programChanged();
    }
}
