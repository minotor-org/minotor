#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"

class MinaExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaExpandingObjects(MinoProgram *program);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Expanding objects", "YYY", QPixmap(), MinaExpandingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaExpandingObjects::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:
    
private:
    MinoProperty _color;
    QGraphicsItemGroup _itemGroup;

};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H