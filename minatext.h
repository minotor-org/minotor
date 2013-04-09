#ifndef MINATEXT_H
#define MINATEXT_H
#include "minoanimation.h"

class MinaText : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaText(MinoAnimationGroup *group);
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("text", "Beat-synced text", QPixmap(":/images/text.png"), MinaText::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return getDescription(); }
    
    void setText(QString value) { _text->setText(value); }
    void setDuration(QString duration) { _beatDuration->setCurrentItem(duration); }
    void setBeatFactor(QString beat) { _beatFactor->setCurrentItem(beat); }
    void setGeneratorStyle(QString style) { _generatorStyle->setCurrentItem(style); }

    QGraphicsItem* graphicItem() { return &_itemGroup; }

signals:
    
public slots:
    
private:
    MinoItemizedProperty *_beatDuration;
    MinoItemizedProperty *_generatorStyle;
    MinoPropertyText *_text;
    QGraphicsItemGroup _itemGroup;
    MinoAnimatedItems _animatedItems;

};

#endif // MINATEXT_H
