#ifndef MINOANIMATIONWAVEFORM_H
#define MINOANIMATIONWAVEFORM_H

#include "minoanimation.h"

class MinaWaveform : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaWaveform(MinoAnimationGroup *group);
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

protected:
    MinoItemizedProperty _colorType;
    MinoPropertyList _mplLine2;
};

#endif // MINOANIMATIONWAVEFORM_H
