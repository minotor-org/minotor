#include "mipromatrix.h"

#include "minotor.h"

#include "minaflash.h"
#include "minarandompixels.h"
#include "minastars.h"
#include "minafallingobjects.h"

// Program "Matrix": animation sets of green effects (Matrix-like)
MiproMatrix::MiproMatrix(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Matrix");

    MinoAnimationGroup *afg = new MinoAnimationGroup(this);
    MinaFlash *af = new MinaFlash(afg);
    af->setColorH(0.4);
    afg->addAnimation(af);
    this->addAnimationGroup(afg);
    MinaRandomPixels *arp = new MinaRandomPixels(afg);
    arp->setColorH(0.4);
    arp->setDensity(0.4);
    afg->addAnimation(arp);


    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaStars *as = new MinaStars(asg);
    as->setColorH(0.4);
    as->setDensity(6);
    asg->addAnimation(as);
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
