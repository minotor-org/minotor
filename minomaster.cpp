#include "minomaster.h"

#include "minotor.h"

#include <QDebug>

MinoMaster::MinoMaster():
    _program(NULL)
{
}

MinoMaster::~MinoMaster()
{

}

void MinoMaster::setBrightness(qreal value)
{
    _program->itemGroup()->setOpacity(value);
}
