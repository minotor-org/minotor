#ifndef MINOMASTER_H
#define MINOMASTER_H

#include "minoprogram.h"

class Minotor;

class MinoMaster : public QObject
{
    Q_OBJECT
public:
    explicit MinoMaster(Minotor *minotor);
    ~MinoMaster();

    void setProgram(MinoProgram *program);
    MinoProgram *program() { return _program; }

    const MinoPropertyList properties() { return _properties; }

private:
    MinoProgram *_program;
    bool _shifted;
    QGraphicsItemGroup _itemGroup;
    MinoPropertyList _properties;

signals:
    void programChanged();

public slots:
    void setBrightness(qreal value);
    void noteChanged(quint8 channel, quint8 note, bool on, quint8 value);
};

#endif // MINOMASTER_H
