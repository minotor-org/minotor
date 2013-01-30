#include "minocue.h"

#include "minotor.h"

#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minadebug.h"
#include "minawaveform.h"
#include "minabarsfromsides.h"

MinoCue::MinoCue(Minotor *minotor, const QRect drawingRect) :
    MinoChannel(minotor, drawingRect)
{
    //this->addAnimation(new MinaExpandingObjects(this));
    //this->addAnimation(new MinaDebug(this));
    //this->addAnimation(new MinaRandomPixels(this));
    //this->addAnimation(new MinaWaveform(this));
    this->addAnimation(new MinaBarsFromSides(minotor));
    //addAnimation("MinaBarsFromSides");
}
