#ifndef MINOANIMATIONPLASMA_H
#define MINOANIMATIONPLASMA_H

#include "minoanimation.h"

class MinaPlasma : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaPlasma(MinoProgram *program);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Plasma", "YYY", QPixmap(), "MinoAnimationPlasma");
    }
    const MinoAnimationDescription description() const { return MinaPlasma::getDescription(); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:

public slots:

private:
    QGraphicsItemGroup _itemGroup;

};

#endif // MINOANIMATIONPLASMA_H
