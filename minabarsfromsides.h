#ifndef MINABARSFROMSIDES_H
#define MINABARSFROMSIDES_H

#include "minoanimation.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaBarsFromSides : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaBarsFromSides(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Bars from sides", "Beat-synced random borders bars", QPixmap(":/images/barsfromsides.png"), MinaBarsFromSides::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

private:
    QGraphicsItemGroup _itemGroup;
    EasingCurvedReal _ecr;
    MinoPropertyEasingCurve *_generatorCurve;

signals:

public slots:

protected:
    MinoItemizedProperty *_colorType;

};

#endif // MINABARSFROMSIDES_H
