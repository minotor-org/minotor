#ifndef MINOANIMATIONRANDOMPIXELS_H
#define MINOANIMATIONRANDOMPIXELS_H

#include "minoanimation.h"
#include "minoanimationproperty.h"

class MinaRandomPixels : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRandomPixels(Minotor *minotor);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Random pixels", "ZZZ", QPixmap(), MinaRandomPixels::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaRandomPixels::getDescription(); }

signals:
    
public slots:

private:
    MinoAnimationProperty _color;
    MinoAnimationProperty _density;
};

#endif // MINOANIMATIONRANDOMPIXELS_H
