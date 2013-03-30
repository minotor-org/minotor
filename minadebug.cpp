#include "minadebug.h"

#include "minopropertycolor.h"

MinaDebug::MinaDebug(MinoAnimationGroup *group) :
    MinoAnimation(group)
{
    new MinoPropertyColor(this);
}

void MinaDebug::animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn)
{
}
