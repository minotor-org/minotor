#include "miprodebug.h"

#include "minacurve.h"
#include "minadebug.h"
#include "minaballs.h"


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

    MinoAnimationGroup *asb = new MinoAnimationGroup(this);
    MinaBalls *ab = new MinaBalls(asb);
    ab->setColor(0.32);
    asb->addAnimation(ab);
    this->addAnimationGroup(asb);
}
