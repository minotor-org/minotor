#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"
#include "easingcurvedreal.h"

class MinaExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaExpandingObjects(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Expanding objects", "Beat-synced expanding objects", QPixmap(":/images/expandingobjects.png"), MinaExpandingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

    void setDuration(QString duration) { _beatDuration->setCurrentItem(duration); }
    QString duration() { return _beatDuration->currentItem()->name(); }

signals:
    
public slots:
    
private:
    MinoItemizedProperty *_beatDuration;
    MinoItemizedProperty *_generatorStyle;
    MinoItemizedProperty *_generatorShape;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
    EasingCurvedReal _ecrScale;
};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H
