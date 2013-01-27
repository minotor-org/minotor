#ifndef MINOANIMATIONDEBUG_H
#define MINOANIMATIONDEBUG_H

#include "minoanimation.h"

class MinoAnimationDebug : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationDebug(MinoChannel *parent);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Debug animation", "### This is tooltip ###", QPixmap(), "MinoAnimationDebug");
    }
    const MinoAnimationDescription description() const { return MinoAnimationDebug::getDescription(); }

private:
    MinoAnimationProperty _r;
    MinoAnimationProperty _g;
    MinoAnimationProperty _b;
signals:
    
public slots:
    
};

#endif // MINOANIMATIONDEBUG_H
