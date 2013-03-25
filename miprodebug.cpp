#include "miprodebug.h"

#include "minacurve.h"

MiproDebug::MiproDebug(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Debug");
    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaCurve *ac = new MinaCurve(asg);
    ac->setColor(0.32);
    asg->addAnimation(ac);
    this->addAnimationGroup(asg);
}
