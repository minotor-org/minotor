#ifndef MINOANIMATIONBARSFROMSIDES_H
#define MINOANIMATIONBARSFROMSIDES_H

#include "minoanimation.h"

class MinaBarsFromSides : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaBarsFromSides(MinoAnimationGroup *group);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Bars from sides", "Beat-synced random borders bars", QPixmap(":/images/barsfromsides.png"), MinaBarsFromSides::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaBarsFromSides::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;

signals:

public slots:
    
};

#endif // MINOANIMATIONBARSFROMSIDES_H
