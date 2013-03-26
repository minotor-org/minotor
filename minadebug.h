#ifndef MINADEBUG_H
#define MINADEBUG_H

#include "minoanimation.h"

class MinaDebug : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaDebug(MinoAnimationGroup *group);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("debug", "Debug", QPixmap(":/images/debug.png"), MinaDebug::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }
    
signals:
    
public slots:

protected:
    QGraphicsItemGroup _itemGroup;

};

#endif // MINADEBUG_H
