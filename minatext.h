#ifndef MINATEXT_H
#define MINATEXT_H
#include "minoanimation.h"

class MinaText : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaText(MinoProgram *program);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("text", "Beat-synced text", QPixmap(":/images/text.png"), MinaText::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaText::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }
    void setText(QString value) { _text.setTextValue(value); }
signals:
    
public slots:
    
private:
    MinoItemizedProperty _beatDuration;
    MinoItemizedProperty _generatorStyle;
    MinoTextProperty _text;
    MinoPropertyList _mplText;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;
};

#endif // MINATEXT_H
