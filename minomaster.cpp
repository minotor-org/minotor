#include "minomaster.h"

#include "minotor.h"

#include <QDebug>

#include "minarandompixels.h"
#include "minaexpandingobjects.h"
#include "minadebug.h"
#include "minawaveform.h"
#include "minabarsfromsides.h"

MinoMaster::MinoMaster(Minotor *minotor, const QRect drawingRect):
    MinoChannel(minotor, drawingRect)
{
    //this->addAnimation(new MinaExpandingObjects(this));
    this->addAnimation(new MinaDebug(minotor));
    //this->addAnimation(new MinaRandomPixels(this));
    //this->addAnimation(new MinaWaveform(this));
    //this->addAnimation(new MinaBarsFromSides(this));
}

MinoMaster::~MinoMaster()
{

}

void MinoMaster::setBrightness(qreal value)
{
    qDebug() << "brightness" << value;
    _itemGroup.setOpacity(value);
}
