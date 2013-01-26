#include "minomaster.h"

#include "minotor.h"

#include <QDebug>

MinoMaster::MinoMaster(Minotor *minotor, const QRect drawingRect):
    MinoChannel(minotor, drawingRect)
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
