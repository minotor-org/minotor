#ifndef MINAFLASHBARS_H
#define MINAFLASHBARS_H

#include "minoanimation.h"

#include "minopropertyreal.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaFlashBars : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaFlashBars(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Flashing bars", "Beat-synced flashing bars", QPixmap(":/images/flashbars.png"), MinaFlashBars::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    void setWidth(qreal density) { _width->setValue(density); }
    QGraphicsItem *graphicItem() { return &_itemGroup; }

signals:
    
public slots:

private:
    QGraphicsItemGroup _itemGroup;
    MinoPropertyReal *_width;
    MinoPropertyEasingCurve *_generatorCurve;
    EasingCurvedReal _ecrAlpha;
    int _oldPosY;
};

#endif // MINAFLASHBARS_H
