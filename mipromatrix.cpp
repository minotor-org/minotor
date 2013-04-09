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
    const QColor matrixColor(QColor::fromHslF(0.32, 1.0, 0.5));

    this->setObjectName("Matrix");

    MinoAnimationGroup *afg = new MinoAnimationGroup(this);
    MinaFlash *af = new MinaFlash(afg);
    af->setColor(matrixColor);
    afg->addAnimation(af);
    MinaRandomPixels *arp = new MinaRandomPixels(afg);
    arp->setColor(matrixColor);
    arp->setDensity(0.4);
    afg->addAnimation(arp);
    this->addAnimationGroup(afg);

    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaStars *as = new MinaStars(asg);
    as->setColor(QColor::fromHslF(0.3, 1.0, 0.5));
    as->setDensity(4);
    as->setDuration("2");
    as->setFrequency("1/2");
    asg->addAnimation(as);
    MinaVibration *av = new MinaVibration(asg);
    av->setColor(QColor::fromHslF(0.32, 1.0, 0.5));
    av->setSegments(0.2);
    asg->addAnimation(av);
    this->addAnimationGroup(asg);

    MinoAnimationGroup *afog = new MinoAnimationGroup(this);
    MinaFallingObjects *afo = new MinaFallingObjects(afog);
    afo->setColor(matrixColor);
    afo->setBeatPeriod("1/2");
    afo->setDensity(7);
    afo->setDuration(8);
    afo->setLength(5);
    afog->addAnimation(afo);
    this->addAnimationGroup(afog);
}
