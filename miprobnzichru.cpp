#include "miprobnzichru.h"

#include "minatext.h"

MiproBnzIchRU::MiproBnzIchRU(Minotor *minotor) : MinoProgram(minotor)
{
    this->setObjectName("Text");
    MinaText *atBad = new MinaText(this);
    atBad->setColorH(0.08);
    atBad->setBeatFactor("1/4");
    atBad->setDuration("1/2");
    atBad->setGeneratorStyle("P:F T:R");
    atBad->setText("BAD");
    this->addAnimation(atBad);

    MinaText *atU = new MinaText(this);
    atU->setBeatFactor("1/4");
    atU->setDuration("1");
    atU->setGeneratorStyle("P:F T:R");
    atU->setColorH(0.02);
    atU->setText("U");
    this->addAnimation(atU);

    MinaText *atX = new MinaText(this);
    atX->setBeatFactor("1/4");
    atX->setDuration("1");
    atX->setGeneratorStyle("P:F T:R");
    atX->setColorH(0.2);
    atX->setText("X");
    this->addAnimation(atX);

    MinaText *atR = new MinaText(this);
    atR->setBeatFactor("1/4");
    atR->setDuration("1");
    atR->setGeneratorStyle("P:F T:R");
    atR->setColorH(0.14);
    atR->setText("R");
    this->addAnimation(atR);
}
