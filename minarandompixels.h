#ifndef MINOANIMATIONRANDOMPIXELS_H
#define MINOANIMATIONRANDOMPIXELS_H

#include "minoanimation.h"

class MinaRandomPixels : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRandomPixels(MinoProgram *program);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Random pixels", "ZZZ", QPixmap(), MinaRandomPixels::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaRandomPixels::getDescription(); }

    void setColorH(qreal hue) { _color.setValue(hue); }
    void setDensity(qreal density) { _density.setValue(density); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:

private:
    MinoProperty _color;
    MinoProperty _density;
    QGraphicsItemGroup _itemGroup;
};

#endif // MINOANIMATIONRANDOMPIXELS_H
