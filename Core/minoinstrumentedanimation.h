#ifndef MINOINSTRUMENTEDANIMATION_H
#define MINOINSTRUMENTEDANIMATION_H

#include "minoanimation.h"

class MinoInstrumentedAnimation : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoInstrumentedAnimation(QObject *parent);
    virtual void createItem() = 0;

signals:
    
public slots:
    virtual void handleNoteChange(int interface, quint8 channel, quint8 note, bool on, quint8 value) = 0;

};

#endif // MINOINSTRUMENTEDANIMATION_H
