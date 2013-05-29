#include "minadebug.h"

#include "minopropertycolor.h"
#include "minopropertyeasingcurve.h"

MinaDebug::MinaDebug(QObject *object) :
    MinoAnimation(object)
{
}

void MinaDebug::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
    (void)uppqn;
    (void)gppqn;
    (void)ppqn;
    (void)qn;
}
