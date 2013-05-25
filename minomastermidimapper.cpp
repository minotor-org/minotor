#include "minomastermidimapper.h"

#include "midimapper.h"

#include "minomaster.h"
#include "minoprogram.h"
#include "minoanimationgroup.h"
#include "minoanimation.h"

MinoMasterMidiMapper::MinoMasterMidiMapper(MinoMaster *parent) :
    QObject(parent),
    _master(parent),
    _program(NULL)
{
    Q_ASSERT(_master);
    connect(_master, SIGNAL(programChanged()), this, SLOT(updateProgram()));

    // HACK to register roles
    clearRoles();
}

void MinoMasterMidiMapper::clearRoles()
{
    // FIXME
    const int columns = 9;
    const int rows = 2;
    const int virtual_pages = 4;
    const int virtual_columns = columns*virtual_pages;

    for (int i=0; i<virtual_columns; i++)
    {
        QString role = QString("MASTER_ANIMATION_%1").arg(i);
        MidiMapper::registerTrigger(role, NULL, NULL, false, true);
        role = QString("MASTER_ANIMATION_SHIFT_%1").arg(i);
        MidiMapper::registerTrigger(role, NULL, NULL, false, true);
    }

    // FIXME
    QSize sHardMappedArea(virtual_columns,rows);
    for (int x=0; x<sHardMappedArea.width(); ++x)
    {
        for(int y=0; y<sHardMappedArea.height(); ++y)
        {
            const QString role = QString("MASTER_CONTROLS_%1_%2").arg(x).arg(y);
            MidiMapper::registerControl(role, NULL, NULL, true);
        }
    }
}

void MinoMasterMidiMapper::updateProgram()
{
    MinoProgram *program = _master->program();
    if(_program)
        unregisterProgram(_program);
    if(program)
        registerProgram(program);
    _program = program;
    updateMap();
}

void MinoMasterMidiMapper::registerProgram(MinoProgram *mp)
{
    connect(mp, SIGNAL(animationGroupAdded(QObject*)), this, SLOT(addGroup(QObject*)));
    connect(mp, SIGNAL(animationGroupMoved(QObject*)), this, SLOT(moveGroup(QObject*)));

    foreach(MinoAnimationGroup *mag, mp->animationGroups())
    {
        registerGroup(mag);
    }
}

void MinoMasterMidiMapper::unregisterProgram(MinoProgram *mp)
{
    disconnect(mp, SIGNAL(animationGroupAdded(QObject*)), this, SLOT(addGroup(QObject*)));
    disconnect(mp, SIGNAL(animationGroupMoved(QObject*)), this, SLOT(moveGroup(QObject*)));

    foreach(MinoAnimationGroup *mag, mp->animationGroups())
    {
        unregisterGroup(mag);
    }
}

void MinoMasterMidiMapper::registerGroup(MinoAnimationGroup *mag)
{
    connect(mag, SIGNAL(animationAdded(QObject*)), this, SLOT(addAnimation(QObject*)));
    connect(mag, SIGNAL(animationMoved(QObject*)), this, SLOT(moveAnimation(QObject*)));

    foreach(MinoAnimation *ma, mag->animations())
    {
        registerAnimation(ma);
    }
}

void MinoMasterMidiMapper::unregisterGroup(MinoAnimationGroup *mag)
{
    disconnect(mag, SIGNAL(animationAdded(QObject*)), this, SLOT(addAnimation(QObject*)));
    disconnect(mag, SIGNAL(animationMoved(QObject*)), this, SLOT(moveAnimation(QObject*)));

    foreach(MinoAnimation *ma, mag->animations())
    {
        unregisterAnimation(ma);
    }
    //FIXME: unregisterTriggers() (eg. MASTER_ANIMATION_)
}

void MinoMasterMidiMapper::registerAnimation(MinoAnimation *ma)
{
    QList<MidiControllableParameter*> mcpl = ma->findChildren<MidiControllableParameter*>();
    foreach (MidiControllableParameter* mcp, mcpl)
    {
        connect(mcp, SIGNAL(attributesChanged()), this, SLOT(changeMidiControllableParameterAttributes()));
    }
}

void MinoMasterMidiMapper::unregisterAnimation(MinoAnimation *ma)
{
    QList<MidiControllableParameter*> mcpl = ma->findChildren<MidiControllableParameter*>();
    foreach (MidiControllableParameter* mcp, mcpl)
    {
        disconnect(mcp, SIGNAL(attributesChanged()), this, SLOT(changeMidiControllableParameterAttributes()));
        //FIXME: unregisterTriggers() (eg. MASTER_CONTROLS_)
    }
}

void MinoMasterMidiMapper::addGroup(QObject *group)
{
    MinoAnimationGroup *mag = qobject_cast<MinoAnimationGroup*>(group);
    Q_ASSERT(mag);
    registerGroup(mag);
    updateMap();
}

void MinoMasterMidiMapper::moveGroup(QObject *group)
{
    MinoAnimationGroup *mag = qobject_cast<MinoAnimationGroup*>(group);
    Q_ASSERT(mag);
    if(mag->program() != _program)
    {
        unregisterGroup(mag);
    }
    updateMap();
}

void MinoMasterMidiMapper::addAnimation(QObject *animation)
{
    MinoAnimation *ma = qobject_cast<MinoAnimation*>(animation);
    Q_ASSERT(ma);
    registerAnimation(ma);
    updateMap();
}

void MinoMasterMidiMapper::moveAnimation(QObject *animation)
{
    MinoAnimation *ma = qobject_cast<MinoAnimation*>(animation);
    Q_ASSERT(ma);
    if(ma->group()->program() != _program)
    {
        unregisterAnimation(ma);
    }
    updateMap();
}

void MinoMasterMidiMapper::changeMidiControllableParameterAttributes()
{
    updateMap();
}

void MinoMasterMidiMapper::updateMap()
{
    clearRoles();
    if (_program)
    {
        // FIXME qMin(_program->animationGroups(), columns)
        for(int i=0; i<_program->animationGroups().count(); ++i)
        {
            MinoAnimationGroup *group = _program->animationGroups().at(i);
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
                    // FIXME qMin(counter, row)
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
    emit updated();
}
