#include "miprowaves.h"
#include "minawaveform.h"

MiproWaves::MiproWaves(Minotor *minotor) :
    MinoProgram(minotor)
{
    this->setObjectName("Waves");
    MinaWaveform *awf = new MinaWaveform(this);
    awf->setColorH(0.1);
    this->addAnimation(awf);

    MinaWaveform *awf1 = new MinaWaveform(this);
    awf1->setColorH(0.3);
    this->addAnimation(awf1);

    MinaWaveform *awf2 = new MinaWaveform(this);
    awf2->setColorH(0.6);
    this->addAnimation(awf2);

    MinaWaveform *awf3 = new MinaWaveform(this);
    awf3->setColorH(0.9);
    this->addAnimation(awf3);
}
