#ifndef MINOANIMATIONWAVEFORM_H
#define MINOANIMATIONWAVEFORM_H

#include "minoanimation.h"
#include "easingcurvedreal.h"
#include "minopropertyeasingcurve.h"

class MinaWaveform : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaWaveform(QObject *object);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Waveform", "Beat-synced waves", QPixmap(":/images/waveform.png"), MinaWaveform::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }
    
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:

private:
    QGraphicsItemGroup _itemGroup;
    EasingCurvedReal _ecrHeight;
    MinoPropertyEasingCurve *_generatorCurve;
    qreal *_linesHeight;

protected:
    MinoItemizedProperty *_colorType;
    MinoItemizedProperty *_animationType;

};

#endif // MINOANIMATIONWAVEFORM_H
