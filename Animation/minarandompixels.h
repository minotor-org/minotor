#ifndef MINARANDOMPIXELS_H
#define MINARANDOMPIXELS_H

#include "minoanimation.h"

#include "minopropertyreal.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaRandomPixels : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRandomPixels(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Random pixels", "Beat-synced randomly placed pixels", QPixmap(":/images/randompixels.png"), MinaRandomPixels::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    void setDensity(qreal density) { _density->setValue(density); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:

private:
    MinoPropertyReal *_density;
    MinoPropertyEasingCurve *_generatorCurve;
    QGraphicsItemGroup _itemGroup;
    EasingCurvedReal _ecrAlpha;
};

#endif // MINARANDOMPIXELS_H
