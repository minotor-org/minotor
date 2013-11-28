#ifndef MINOINSTRUMENTEDANIMATION_H
#define MINOINSTRUMENTEDANIMATION_H

#include "minoanimation.h"

class MinoInstrumentedAnimation : public MinoAnimation
{
    Q_OBJECT
public:
    explicit MinoInstrumentedAnimation(QObject *parent);
    virtual void createItem() = 0;

    static QColor noteToColor(const quint8 note);

    bool isAlive() const { return _alive; }

signals:
    
public slots:
    virtual void handleNoteChange(int interface, quint8 channel, quint8 note, bool on, quint8 value) = 0;

protected:
    // Status
    bool _alive;
    virtual void setAlive(const bool on);

};

#endif // MINOINSTRUMENTEDANIMATION_H
