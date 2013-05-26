#ifndef MINAPLASMA_H
#define MINAPLASMA_H

#include "minoanimation.h"

#include <QGraphicsRectItem>

#include "minopropertyeasingcurve.h"
#include "easingcurvedreal.h"

class MinaPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaPlasma(QObject *object);
    ~MinaPlasma();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("plasma", "Plasma", QPixmap(":/images/plasma.png"), MinaPlasma::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    QGraphicsRectItem *_rectBackground;
    QGraphicsRectItem *_rectTopLeft;
    QGraphicsRectItem *_rectTopRight;
    QGraphicsRectItem *_rectBottomLeft;
    QGraphicsRectItem *_rectBottomRight;
    MinoPropertyEasingCurve *_generatorCurve;
    MinoItemizedProperty *_generatorSteps;
    EasingCurvedReal _ecrPosition;
};

#endif // MINAPLASMA_H
