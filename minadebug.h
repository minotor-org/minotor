#ifndef MINOANIMATIONDEBUG_H
#define MINOANIMATIONDEBUG_H

#include "minoanimation.h"

class MinaDebug : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaDebug(Minotor *minotor);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Debug animation", "### This is tooltip ###", QPixmap(":/animations/res/flash.png"), MinaDebug::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaDebug::getDescription(); }

private:
    MinoProperty _color;

signals:
    
public slots:
    
};

#endif // MINOANIMATIONDEBUG_H
