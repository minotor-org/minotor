#ifndef MINOANIMATIONBARSFROMSIDES_H
#define MINOANIMATIONBARSFROMSIDES_H

#include "minoanimation.h"

class MinaBarsFromSides : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinaBarsFromSides(Minotor *minotor);
    void animate(const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    static const MinoAnimationDescription getDescription() {
        return MinoAnimationDescription("Bars from sides", "XXX", QPixmap(), MinaBarsFromSides::staticMetaObject.className());
    }
    const MinoAnimationDescription description() const { return MinaBarsFromSides::getDescription(); }

private:
    MinoProperty _color;
signals:

public slots:
    
};

#endif // MINOANIMATIONBARSFROMSIDES_H
