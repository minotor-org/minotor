#include "minomaster.h"

#include "minotor.h"
#include "minoanimationgroup.h"
#include "minopropertyreal.h"
#include "midimapper.h"

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

    // FIXME
    for (int i=0; i<100; i++)
    {
        QString role = QString("MASTER_ANIMATION_%1").arg(i);
        MidiMapper::registerTrigger(role);
        role = QString("MASTER_ANIMATION_SHIFT_%1").arg(i);
        MidiMapper::registerTrigger(role);
    }

    // FIXME
    QSize sHardMappedArea(9*4,2);

    for (int x=0; x<sHardMappedArea.width(); ++x)
    {
        for(int y=0; y<sHardMappedArea.height(); ++y)
        {
            const QString role = QString("MASTER_CONTROLS_%1_%2").arg(x).arg(y);
            MidiMapper::registerControl(role);
        }
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

            for(int i=0; i<program->animationGroups().count(); ++i)
            {
                MinoAnimationGroup *group = program->animationGroups().at(i);
                QString role = QString("MASTER_ANIMATION_%1").arg(i);
                MidiMapper::registerTrigger(role, group, SLOT(setEnabled(bool)), true, true);
                role = QString("MASTER_ANIMATION_SHIFT_%1").arg(i);
                MidiMapper::registerTrigger(role, group, SLOT(toogle()), false, true);

                int id = 0;
                foreach(MinoAnimation *animation, group->animations())
                {
                    QList<MidiControllableParameter*> mcp = animation->findChildren<MidiControllableParameter*>();
                    for(int j=0; j<mcp.count(); ++j)
                    {
                        if(mcp.at(j)->isPreferred())
                        {
                            const QString role = QString("MASTER_CONTROLS_%1_%2").arg(i).arg(id);
                            MidiMapper::registerControl(role, mcp.at(j), SLOT(setValueFromMidi(quint8)), true);
                            id++;
                        }
                    }
                }
            }
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
