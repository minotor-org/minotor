#ifndef MINAROTATINGBARS_H
#define MINAROTATINGBARS_H
#include "minoanimation.h"

class MinaRotatingBars : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRotatingBars(MinoAnimationGroup *group);
    ~MinaRotatingBars();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("plasma", "Plasma", QPixmap(":/images/rotatingbars.png"), MinaRotatingBars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    QGraphicsRectItem *_items[4];
    MinoPropertyList _mplLine2;
    MinoPropertyList _mplLine3;
    MinoItemizedProperty _generatorCurve;
    MinoProperty _width;
    MinoProperty _length;
    int _maxPixels;
    QRectF _drawingZone;
};

#endif // MINAROTATINGBARS_H
