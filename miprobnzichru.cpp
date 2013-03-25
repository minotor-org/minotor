#include "miprobnzichru.h"

#include "minatext.h"

MiproBnzIchRU::MiproBnzIchRU(Minotor *minotor) : MinoProgram(minotor)
{
    this->setObjectName("Text");
    MinoAnimationGroup *atBadg = new MinoAnimationGroup(this);
    MinaText *atBad = new MinaText(atBadg);
    atBad->setColor(0.08);
    atBad->setBeatFactor("1/4");
    atBad->setDuration("1/2");
    atBad->setGeneratorStyle("P:F T:R");
    atBad->setText("BAD");
    atBadg->addAnimation(atBad);
    this->addAnimationGroup(atBadg);

    MinoAnimationGroup *atUg = new MinoAnimationGroup(this);
    MinaText *atU = new MinaText(atUg);
    atU->setBeatFactor("1/4");
    atU->setDuration("1");
    atU->setGeneratorStyle("P:F T:R");
    atU->setColor(0.02);
    atU->setText("U");
    atUg->addAnimation(atU);
    this->addAnimationGroup(atUg);

    MinoAnimationGroup *atXg = new MinoAnimationGroup(this);
    MinaText *atX = new MinaText(atXg);
    atX->setBeatFactor("1/4");
    atX->setDuration("1");
    atX->setGeneratorStyle("P:F T:R");
    atX->setColor(0.2);
    atX->setText("X");
    atXg->addAnimation(atX);
    this->addAnimationGroup(atXg);

    MinoAnimationGroup *atRg = new MinoAnimationGroup(this);
    MinaText *atR = new MinaText(atRg);
    atR->setBeatFactor("1/4");
    atR->setDuration("1");
    atR->setGeneratorStyle("P:F T:R");
    atR->setColor(0.14);
    atR->setText("R");
    atRg->addAnimation(atR);
    this->addAnimationGroup(atRg);
}
