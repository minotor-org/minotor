#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"

class MinoAnimationExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoAnimationExpandingObjects(MinoChannel *channel);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Expanding objects", "YYY", QPixmap(), "MinoAnimationExpandingObjects");
    }
    const MinoAnimationDescription description() const { return MinoAnimationExpandingObjects::getDescription(); }
signals:
    
public slots:
    
private:
    MinoAnimationProperty _color;
};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H
