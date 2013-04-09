#include "miprosecondlives.h"

#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minaflash.h"
#include "minastars.h"
#include "minafallingobjects.h"
#include "minarotatingbars.h"

MiproSecondLives::MiproSecondLives(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Vitalic - Second Lives");
    MinoAnimationGroup *agrp = new MinoAnimationGroup(this);
    MinaRandomPixels *arp = new MinaRandomPixels(agrp);
    arp->setColor(QColor::fromHslF(0.5, 1.0, 0.5));
    arp->setFrequency("1");
    arp->setDensity(0.01);
    agrp->addAnimation(arp);
    this->addAnimationGroup(agrp);

    MinoAnimationGroup *ageo = new MinoAnimationGroup(this);
    MinaExpandingObjects *aeo = new MinaExpandingObjects(ageo);
    aeo->setColor(QColor::fromHslF(0.25, 1.0, 0.5));
    aeo->setFrequency("8");
    aeo->setDuration("8");
    ageo->addAnimation(aeo);
    this->addAnimationGroup(ageo);

    MinoAnimationGroup *agf = new MinoAnimationGroup(this);
    MinaFlash *af = new MinaFlash(agf);
    af->setColor(QColor::fromHslF(0.6, 1.0, 0.5));
    af->setFrequency("1");
    agf->addAnimation(af);
    this->addAnimationGroup(agf);

    MinoAnimationGroup *ags1 = new MinoAnimationGroup(this);
    MinaStars *as1 = new MinaStars(ags1);
    as1->setColor(QColor::fromHslF(0.65, 1.0, 0.5));
    as1->setFrequency("1/4");
    as1->setDuration("2");
    as1->setDensity(5);
    ags1->addAnimation(as1);
    this->addAnimationGroup(ags1);

    MinoAnimationGroup *ags2 = new MinoAnimationGroup(this);
    MinaStars *as2 = new MinaStars(ags2);
    as2->setColor(QColor::fromHslF(0.8, 1.0, 0.5));
    as2->setFrequency("1/4");
    as2->setDuration("2");
    as2->setDensity(5);
    ags2->addAnimation(as2);
    this->addAnimationGroup(ags2);

    MinoAnimationGroup *agf2 = new MinoAnimationGroup(this);
    MinaFlash *af2 = new MinaFlash(agf);
    af2->setColor(QColor::fromHslF(0.2, 1.0, 0.5));
    af2->setFrequency("16");
    agf2->addAnimation(af2);
    this->addAnimationGroup(agf2);

    MinoAnimationGroup *agfo1 = new MinoAnimationGroup(this);
    MinaFallingObjects *afo1 = new MinaFallingObjects(agfo1);
    afo1->setColor(QColor::fromHslF(0.4, 1.0, 0.5));
    afo1->setFrequency("1/2");
    afo1->setDirection("Left");
    afo1->setDensity(1);
    agfo1->addAnimation(afo1);
    this->addAnimationGroup(agfo1);

    MinoAnimationGroup *ags3 = new MinoAnimationGroup(this);
    MinaStars *as3 = new MinaStars(ags3);
    as3->setColor(QColor::fromHslF(0.4, 1.0, 0.5));
    as3->setFrequency("1/4");
    as3->setDuration("2");
    as3->setDensity(5);
    ags3->addAnimation(as3);
    this->addAnimationGroup(ags3);

    MinoAnimationGroup *agfo2 = new MinoAnimationGroup(this);
    MinaFallingObjects *afo2 = new MinaFallingObjects(agfo2);
    afo2->setColor(QColor::fromHslF(0.6, 1.0, 0.5));
    afo2->setFrequency("1/2");
    afo2->setDirection("Right");
    afo2->setDensity(1);
    agfo2->addAnimation(afo2);
    this->addAnimationGroup(agfo2);

    MinoAnimationGroup *agfo3 = new MinoAnimationGroup(this);
    MinaFallingObjects *afo3 = new MinaFallingObjects(agfo3);
    afo3->setColor(QColor::fromHslF(0.25, 1.0, 0.5));
    afo3->setFrequency("8");
    afo3->setDuration(16);
    afo3->setDirection("Down");
    afo3->setDensity(6);
    agfo3->addAnimation(afo3);
    MinaFallingObjects *afo4 = new MinaFallingObjects(agfo3);
    afo4->setColor(1.0);
    afo4->setFrequency("1/2");
    afo4->setDuration(2);
    afo4->setDirection("Up");
    afo4->setDensity(1);
    agfo3->addAnimation(afo4);
    this->addAnimationGroup(agfo3);

    MinoAnimationGroup *agrb1 = new MinoAnimationGroup(this);
    MinaRotatingBars *arb1 = new MinaRotatingBars(agrb1);
    arb1->setColor(QColor::fromHslF(0.6, 1.0, 0.5));
    arb1->setFrequency("1");
    arb1->setWidth(0.25);
    arb1->setLength(0.65);
    arb1->setEasingCurveType(QEasingCurve::OutInBack);
    agrb1->addAnimation(arb1);
    this->addAnimationGroup(agrb1);
}
