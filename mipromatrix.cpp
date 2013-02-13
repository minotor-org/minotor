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

    MinaFlash *af = new MinaFlash(this);
    af->setColorH(0.4);
    this->addAnimation(af);

    MinaRandomPixels *arp = new MinaRandomPixels(this);
    arp->setColorH(0.4);
    arp->setDensity(0.4);
    this->addAnimation(arp);

    MinaStars *as = new MinaStars(this);
    as->setColorH(0.4);
    as->setDensity(6);
    this->addAnimation(as);

    MinaFallingObjects *afo = new MinaFallingObjects(this);
    afo->setColorH(0.4);
    afo->setBeatPeriod("1/2");
    afo->setDensity(7);
    afo->setDuration(8);
    afo->setLength(5);
    this->addAnimation(afo);
}
