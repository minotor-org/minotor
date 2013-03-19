#include "mipromatrix.h"

#include "minotor.h"

#include "minaflash.h"
#include "minarandompixels.h"
#include "minastars.h"
#include "minafallingobjects.h"
#include "minavibration.h"

// Program "Matrix": animation sets of green effects (Matrix-like)
MiproMatrix::MiproMatrix(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Matrix");

    MinoAnimationGroup *afg = new MinoAnimationGroup(this);
    MinaFlash *af = new MinaFlash(afg);
    af->setColorH(0.4);
    afg->addAnimation(af);
    MinaRandomPixels *arp = new MinaRandomPixels(afg);
    arp->setColorH(0.4);
    arp->setDensity(0.4);
    afg->addAnimation(arp);
    this->addAnimationGroup(afg);

    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaStars *as = new MinaStars(asg);
    as->setColorH(0.3);
    as->setDensity(4);
    as->setDuration("2");
    as->setFrequency("1/2");
    asg->addAnimation(as);
    MinaVibration *av = new MinaVibration(asg);
    av->setColorH(0.32);
    av->setSegments(0.2);
    asg->addAnimation(av);
    this->addAnimationGroup(asg);

    MinoAnimationGroup *afog = new MinoAnimationGroup(this);
    MinaFallingObjects *afo = new MinaFallingObjects(afog);
    afo->setColorH(0.4);
    afo->setBeatPeriod("1/2");
    afo->setDensity(7);
    afo->setDuration(8);
    afo->setLength(5);
    afog->addAnimation(afo);
    this->addAnimationGroup(afog);

}
