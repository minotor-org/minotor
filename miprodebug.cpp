#include "miprodebug.h"

#include "minarotatingbars.h"

MiproDebug::MiproDebug(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Debug");
    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaRotatingBars *arb = new MinaRotatingBars(asg);
    arb->setColor(0.32);
    asg->addAnimation(arb);
    this->addAnimationGroup(asg);
}
