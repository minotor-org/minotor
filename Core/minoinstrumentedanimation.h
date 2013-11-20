#ifndef MINOINSTRUMENTEDANIMATION_H
#define MINOINSTRUMENTEDANIMATION_H

#include "minoanimation.h"

class MinoInstrumentedAnimation : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoInstrumentedAnimation(QObject *parent);
    virtual void createItem() {}
    
signals:
    
public slots:
    
};

#endif // MINOINSTRUMENTEDANIMATION_H
