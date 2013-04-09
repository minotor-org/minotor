#include "miprodebug.h"

#include "minacurve.h"
#include "minadebug.h"

MiproDebug::MiproDebug(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Debug");
    MinoAnimationGroup *asgd = new MinoAnimationGroup(this);
    MinaDebug *ad = new MinaDebug(asgd);
    asgd->addAnimation(ad);
    this->addAnimationGroup(asgd);

    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaCurve *ac = new MinaCurve(asg);
    ac->setColor(QColor::fromHslF(0.32, 1.0, 0.5));
    asg->addAnimation(ac);
    this->addAnimationGroup(asg);
}
