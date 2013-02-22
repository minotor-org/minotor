#ifndef MINOANIMATIONPLASMA_H
#define MINOANIMATIONPLASMA_H

#include "minoanimation.h"
#include <QGraphicsRectItem>

class MinaPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaPlasma(MinoProgram *program);
    ~MinaPlasma();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("plasma", "Plasma", QPixmap(":/images/plasma.png"), MinaPlasma::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaPlasma::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    QGraphicsRectItem *_rectBackground;
    QGraphicsRectItem *_rectTopLeft;
    QGraphicsRectItem *_rectTopRight;
    QGraphicsRectItem *_rectBottomLeft;
    QGraphicsRectItem *_rectBottomRight;
    MinoPropertyList _mplLine2;
    MinoItemizedProperty _generatorCurve;
    MinoItemizedProperty _generatorSteps;
};

#endif // MINOANIMATIONPLASMA_H
