#include "miprowaves.h"
#include "minawaveform.h"

MiproWaves::MiproWaves(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Waves");
    MinoAnimationGroup *awfg = new MinoAnimationGroup(this);
    MinaWaveform *awf = new MinaWaveform(awfg);
    awf->setColor(0.1);
    awfg->addAnimation(awf);
    this->addAnimationGroup(awfg);

    MinoAnimationGroup *awf1g = new MinoAnimationGroup(this);
    MinaWaveform *awf1 = new MinaWaveform(awf1g);
    awf1->setColor(0.3);
    awf1g->addAnimation(awf1);
    this->addAnimationGroup(awf1g);

    MinoAnimationGroup *awf2g = new MinoAnimationGroup(this);
    MinaWaveform *awf2 = new MinaWaveform(awf2g);
    awf2->setColor(0.6);
    awf2g->addAnimation(awf2);
    this->addAnimationGroup(awf2g);

    MinoAnimationGroup *awf3g = new MinoAnimationGroup(this);
    MinaWaveform *awf3 = new MinaWaveform(awf3g);
    awf3->setColor(0.9);
    awf3g->addAnimation(awf3);
    this->addAnimationGroup(awf3g);
}
