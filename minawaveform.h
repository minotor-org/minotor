#ifndef MINOANIMATIONWAVEFORM_H
#define MINOANIMATIONWAVEFORM_H

#include "minoanimation.h"

class MinaWaveform : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaWaveform(MinoProgram *program);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Waveform", "YYY", QPixmap(), MinaWaveform::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaWaveform::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }
    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:

private:
    MinoProperty _color;
    QGraphicsItemGroup _itemGroup;
};

#endif // MINOANIMATIONWAVEFORM_H
