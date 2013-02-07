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

    MinaFlash *af1 = new MinaFlash(this);
    af1->setColorH(0.4);
    this->addAnimation(af1);

    MinaRandomPixels *arp1 = new MinaRandomPixels(this);
    arp1->setColorH(0.4);
    arp1->setDensity(0.4);
    this->addAnimation(arp1);

    MinaWaveform *awf1 = new MinaWaveform(this);
    awf1->setColorH(0.4);
    this->addAnimation(awf1);

    MinaBarsFromSides *abfs1 = new MinaBarsFromSides(this);
    abfs1->setColorH(0.4);
    this->addAnimation(abfs1);

    MinaExpandingObjects *aeo1 = new MinaExpandingObjects(this);
    aeo1->setColorH(0.4);
    this->addAnimation(aeo1);

    MinaFlash *af2 = new MinaFlash(this);
    af2->setColorH(0.4);
    this->addAnimation(af2);

    MinaRandomPixels *arp2 = new MinaRandomPixels(this);
    arp2->setColorH(0.4);
    arp2->setDensity(0.4);
    this->addAnimation(arp2);

    MinaWaveform *awf2 = new MinaWaveform(this);
    awf2->setColorH(0.4);
    this->addAnimation(awf2);

    MinaBarsFromSides *abfs2 = new MinaBarsFromSides(this);
    abfs2->setColorH(0.4);
    this->addAnimation(abfs2);

    MinaExpandingObjects *aeo2 = new MinaExpandingObjects(this);
    aeo2->setColorH(0.4);
    this->addAnimation(aeo2);
}
