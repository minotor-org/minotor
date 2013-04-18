#ifndef MINASTARS_H
#define MINASTARS_H

#include "minoanimation.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaStars : public MinoAnimation
{
public:
    explicit MinaStars(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("stars", "Moving stars", QPixmap(":/images/stars.png"), MinaStars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    void setDensity(const unsigned int density) { _generatorDensity->setCurrentItem(QString::number(density)); }
    void setDuration(QString duration) { _beatDuration->setCurrentItem(duration);}

    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:

public slots:

private:
    MinoItemizedProperty *_beatDuration;
    MinoItemizedProperty *_generatorDensity;
    MinoPropertyEasingCurve *_generatorCurve;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
    EasingCurvedReal _ecrPosition;
};

#endif // MINASTARS_H
