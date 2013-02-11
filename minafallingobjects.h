#ifndef MINAFALLINGOBJECTS_H
#define MINAFALLINGOBJECTS_H
#include "minoanimation.h"

class MinaFallingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaFallingObjects(MinoProgram *program);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("falling", "Falling Objects", QPixmap(":/images/falling.png"), MinaFallingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaFallingObjects::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }
    
signals:
    
public slots:

protected:
    MinoItemizedProperty _beatDuration;
    MinoItemizedProperty _generatorDirection;
    MinoItemizedProperty _generatorLength;
    MinoItemizedProperty _generatorDensity;
    MinoPropertyList _mplStyle;
    MinoPropertyList _mplLength;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
};
#endif // MINAFALLINGOBJECTS_H
