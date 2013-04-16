#ifndef MINAROTATINGBARS_H
#define MINAROTATINGBARS_H

#include "minoanimation.h"

#include "minopropertyreal.h"
#include "minopropertyeasingcurve.h"

class MinaRotatingBars : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaRotatingBars(QObject *object);
    ~MinaRotatingBars();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("plasma", "Plasma", QPixmap(":/images/rotatingbars.png"), MinaRotatingBars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    void setWidth(const qreal width) { _width->setValue(width); }
    void setLength(const qreal length) { _length->setValue(length); }
    void setEasingCurveType(const QEasingCurve::Type &type) { _generatorCurve->setEasingCurveType(type); }

    QGraphicsItem *graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    QGraphicsRectItem *_items[4];
    MinoPropertyEasingCurve *_generatorCurve;
    MinoPropertyReal *_width;
    MinoPropertyReal *_length;
    int _maxPixels;
    QRectF _drawingZone;

};

#endif // MINAROTATINGBARS_H
