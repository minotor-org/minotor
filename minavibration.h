#ifndef MINAVIBRATION_H
#define MINAVIBRATION_H

#include "minoanimation.h"

class MinaVibration : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaVibration(MinoAnimationGroup *group);

    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Vibration", "Beat-synced vibration", QPixmap(":/images/vibration.png"), MinaVibration::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }
    
    void setSegments(qreal value) { _segments.setValue(value); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:

public slots:

private:
    QGraphicsItemGroup _itemGroup;

protected:
    MinoPropertyReal _segments;
    MinoPropertyList _mplLine2;
};


#endif // MINAVIBRATION_H
