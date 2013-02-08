#include "miprotext.h"
#include "minatext.h"

MiproText::MiproText(Minotor *minotor) : MinoProgram(minotor)
{

    this->setObjectName("Text");
    MinaText *at = new MinaText(this);
    at->setColorH(0.08);
    at->setText("text");
    this->addAnimation(at);

}
