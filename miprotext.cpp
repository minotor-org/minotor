#include "miprotext.h"
#include "minatext.h"
#include "minafallingobjects.h"
#include "minaexpandingobjects.h"
#include "minabarsfromsides.h"

MiproText::MiproText(Minotor *minotor) : MinoProgram(minotor)
{

    this->setObjectName("Text");
    MinaExpandingObjects *aexp = new MinaExpandingObjects(this);
    aexp->setColorH(0.08);
    this->addAnimation(aexp);

    MinaFallingObjects *afo = new MinaFallingObjects(this);
    afo->setColorH(0.8);
    this->addAnimation(afo);

    MinaBarsFromSides *abfs = new MinaBarsFromSides(this);
    abfs->setColorH(0.6);
    this->addAnimation(abfs);
}
