#ifndef MINOANIMATIONEXPANDINGOBJECTS_H
#define MINOANIMATIONEXPANDINGOBJECTS_H

#include "minoanimation.h"

class MinaExpandingObjects : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaExpandingObjects(Minotor *minotor);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Expanding objects", "YYY", QPixmap(), MinaExpandingObjects::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaExpandingObjects::getDescription(); }
    void setColorH(qreal hue) { _color.setValue(hue); }

signals:
    
public slots:
    
private:
    MinoProperty _color;
};

#endif // MINOANIMATIONEXPANDINGOBJECTS_H
