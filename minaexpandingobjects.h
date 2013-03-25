#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"

class MinaExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaExpandingObjects(MinoAnimationGroup *group);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Expanding objects", "Beat-synced expanding objects", QPixmap(":/images/expandingobjects.png"), MinaExpandingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:
    
private:
    MinoItemizedProperty _beatDuration;
    MinoItemizedProperty _generatorStyle;
    MinoItemizedProperty _generatorShape;
    MinoPropertyList _mplLine2;
    MinoPropertyList _mplLine3;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H
