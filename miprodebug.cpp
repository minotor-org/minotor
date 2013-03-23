#include "miprodebug.h"

#include "minavibration.h"

MiproDebug::MiproDebug(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Debug");
    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaVibration *av = new MinaVibration(asg);
    av->setColorH(0.32);
    av->setSegments(0.2);
    asg->addAnimation(av);
    this->addAnimationGroup(asg);
}
