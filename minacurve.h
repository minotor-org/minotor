#ifndef MINACURVE_H
#define MINACURVE_H

#include "minoanimation.h"

#include "minopropertyeasingcurve.h"

class MinaCurve : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaCurve(QObject *object);
    ~MinaCurve();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Curve", "Scrolling curve", QPixmap(":/images/curve.png"), MinaCurve::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }
signals:

public slots:
private:
    QGraphicsItemGroup _itemGroup;
    QList<QGraphicsLineItem*> _items;
    MinoPropertyEasingCurve *_generatorCurve;
    MinoPropertyEasingCurve *_generatorAccel;

};


#endif // MINACURVE_H
