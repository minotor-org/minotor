#include "minocue.h"

#include "minotor.h"

#include "minoanimationrandompixels.h"
#include "minoanimationexpandingobjects.h"
#include "minoanimationdebug.h"
#include "minoanimationwaveform.h"
#include "minoanimationbarsfromsides.h"

MinoCue::MinoCue(Minotor *minotor, const QRect drawingRect) :
    MinoChannel(minotor, drawingRect)
{
    //this->addAnimation(new MinoAnimationExpandingObjects(this));
    //this->addAnimation(new MinoAnimationDebug(this));
    //this->addAnimation(new MinoAnimationRandomPixels(this));
    //this->addAnimation(new MinoAnimationWaveform(this));
    this->addAnimation(new MinoAnimationBarsFromSides(this));
    //addAnimation("MinoAnimationBarsFromSides");
}
