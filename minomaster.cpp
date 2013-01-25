#include "minomaster.h"

#include "minotor.h"

#include <QDebug>

MinoMaster::MinoMaster(Minotor *minotor):
    MinoChannel(minotor)
{
}

MinoMaster::~MinoMaster()
{

}

void MinoMaster::setBrightness(qreal value)
{
    qDebug() << "brightness" << value;
    _itemGroup.setOpacity(value);
}
