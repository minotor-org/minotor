#include "miprodebug.h"

#include "minotor.h"

#include "minadebug.h"
#include "minaflash.h"
#include "minaimage.h"
#include "minaballs.h"

MiproDebug::MiproDebug(MinoProgramBank *bank) :
    MinoProgram(bank)
{
    this->setLabel("Debug");
    MinoAnimationGroup *asgd = new MinoAnimationGroup(this);
    MinaFlash *ad = new MinaFlash(asgd);
    ad->setColor(QColor::fromHslF(0.12,0.34,0.56));
    ad->setLoopSize("1/4");
    asgd->addAnimation(ad);
    this->addAnimationGroup(asgd);
/*
    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaImage *ai = new MinaImage(asg);
    ai->setColor(QColor::fromHslF(0.32, 1.0, 0.5));
    asg->addAnimation(ai);
    this->addAnimationGroup(asg);

    MinoAnimationGroup *asb = new MinoAnimationGroup(this);
    MinaBalls *ab = new MinaBalls(asb);
    ab->setColor(0.32);
    asb->addAnimation(ab);
    this->addAnimationGroup(asb);
*/
}
