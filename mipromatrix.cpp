#include "mipromatrix.h"

#include "minotor.h"

#include "minaflash.h"
#include "minarandompixels.h"
#include "minawaveform.h"
#include "minaexpandingobjects.h"
#include "minabarsfromsides.h"

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

    MinaWaveform *awf = new MinaWaveform(this);
    awf->setColorH(0.4);
    this->addAnimation(awf);

    MinaBarsFromSides *abfs = new MinaBarsFromSides(this);
    abfs->setColorH(0.4);
    this->addAnimation(abfs);

    MinaExpandingObjects *aeo = new MinaExpandingObjects(this);
    aeo->setColorH(0.4);
    this->addAnimation(aeo);
}
