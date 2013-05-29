#ifndef MINAFALLINGOBJECTS_H
#define MINAFALLINGOBJECTS_H

#include "minoanimation.h"

#include "easingcurvedreal.h"

#include "minopropertyreal.h"
#include "minopropertyeasingcurve.h"

class MinaFallingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaFallingObjects(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Falling objects", "Beat-sync moving lines", QPixmap(":/images/falling.png"), MinaFallingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    // To be deleted...
    void setDuration(const unsigned int duration) { _beatDuration->setCurrentItem(QString::number(duration)); }
    void setDensity(const unsigned int density) { _generatorDensity->setValue(density); }
    void setLength(const unsigned int length) { _generatorLength->setCurrentItem(QString::number(length)); }
    void setDirection(QString direction) { _generatorDirection->setCurrentItem(direction); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

    bool isAlive() const { return _alive; }
    
signals:
    
public slots:

protected:
    MinoItemizedProperty *_beatDuration;
    MinoItemizedProperty *_generatorDirection;
    MinoItemizedProperty *_generatorLength;
    MinoPropertyReal *_generatorDensity;
    MinoPropertyEasingCurve *_generatorCurve;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
    EasingCurvedReal _ecrPosition;
    bool _alive;

    void setAlive(const bool on);

    enum ItemData { Direction };
};
#endif // MINAFALLINGOBJECTS_H
