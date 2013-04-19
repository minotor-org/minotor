#ifndef MINAFLASH_H
#define MINAFLASH_H

#include "minoanimation.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

#include <QGraphicsRectItem>

class MinaFlash : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaFlash(QObject *object);
    ~MinaFlash();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Flash", "Beat-sync flash", QPixmap(":/images/flash.png"), MinaFlash::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem *graphicItem() { return _rectItem; }

private:
    QGraphicsRectItem *_rectItem;
    EasingCurvedReal _ecrOpacity;
    MinoPropertyEasingCurve *_generatorCurve;
signals:
    
public slots:
    
};

#endif // MINAFLASH_H
