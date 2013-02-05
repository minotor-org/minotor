#include "mipromatrix.h"

#include "minotor.h"

#include "minarandompixels.h"
#include "minawaveform.h"
#include "minaexpandingobjects.h"
#include "minabarsfromsides.h"

// Program "Matrix": animation sets of green effects (Matrix-like)
MiproMatrix::MiproMatrix(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Matrix");

    MinaRandomPixels *arp = new MinaRandomPixels(minotor);
    // Green
    arp->setColorH(0.4);
    arp->setDensity(0.4);
    this->addAnimation(arp);

    MinaWaveform *awf = new MinaWaveform(minotor);
    awf->setColorH(0.4);
    this->addAnimation(awf);

    MinaBarsFromSides *abfs = new MinaBarsFromSides(minotor);
    abfs->setColorH(0.4);
    this->addAnimation(abfs);

    MinaExpandingObjects *aeo = new MinaExpandingObjects(minotor);
    aeo->setColorH(0.4);
    this->addAnimation(aeo);
}
