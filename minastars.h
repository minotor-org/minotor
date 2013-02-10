#ifndef MINASTARS_H
#define MINASTARS_H
#include "minoanimation.h"

class MinaStars : public MinoAnimation
{
public:
    explicit MinaStars(MinoProgram *program);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("stars", "Moving stars", QPixmap(":/images/stars.png"), MinaStars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaStars::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:

public slots:

private:
    MinoProperty _color;
    MinoItemizedProperty _beatDuration;
    MinoItemizedProperty _generatorLength;
    MinoItemizedProperty _generatorDensity;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
};

#endif // MINASTARS_H
