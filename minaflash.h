#ifndef MINAFLASH_H
#define MINAFLASH_H

#include "minoanimation.h"

class MinaFlash : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaFlash(Minotor *minotor);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Flash", "### This is tooltip ###", QPixmap(":/animations/res/flash.png"), MinaFlash::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaFlash::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

private:
    MinoProperty _color;

signals:
    
public slots:
    
};

#endif // MINAFLASH_H
