#ifndef MINASTARS_H
#define MINASTARS_H
#include "minoanimation.h"

class MinaStars : public MinoAnimation
{
public:
    explicit MinaStars(MinoAnimationGroup *group);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("stars", "Moving stars", QPixmap(":/images/stars.png"), MinaStars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaStars::getDescription(); }

    void setColorH(qreal hue) { _color.setValue(hue); }
    void setDensity(const unsigned int density) { _generatorDensity.setCurrentItem(QString::number(density)); }
    void setDuration(QString duration) { _beatDuration.setCurrentItem(duration);}

    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:

public slots:

private:
    MinoItemizedProperty _beatDuration;
    MinoItemizedProperty _generatorDensity;
    MinoPropertyList _mplDensity;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;

};

#endif // MINASTARS_H
