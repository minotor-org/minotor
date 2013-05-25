#ifndef MINAGRADIENT_H
#define MINAGRADIENT_H

#include "minoanimation.h"

#include <QGraphicsRectItem>

#include "minopropertyeasingcurve.h"

class MinaGradient : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaGradient(QObject *object);
    ~MinaGradient();
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("gradient", "Gradient", QPixmap(":/images/gradient.png"), MinaGradient::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem *graphicItem() { return _rectItem; }

private:
    QGraphicsRectItem *_rectItem;
    MinoItemizedProperty *_generatorStyle;
    MinoItemizedProperty *_waves;
    MinoPropertyEasingCurve *_generatorCurve;

};

#endif // MINAGRADIENT_H
