#include "miprotext.h"
#include "minatext.h"
#include "minafallingobjects.h"

MiproText::MiproText(Minotor *minotor) : MinoProgram(minotor)
{

    this->setObjectName("Text");
    MinaText *at = new MinaText(this);
    at->setColorH(0.08);
    at->setText("text");
    this->addAnimation(at);

    this->setObjectName("Falling");
    MinaFallingObjects *afo = new MinaFallingObjects(this);
    afo->setColorH(0.8);
    this->addAnimation(afo);

}
