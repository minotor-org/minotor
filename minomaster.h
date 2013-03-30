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

private:
    MinoProgram *_program;
    bool _shifted;
    QGraphicsItemGroup _itemGroup;

signals:
    void programChanged();
    void updated();

public slots:
    void setBrightness(qreal value);

};

#endif // MINOMASTER_H
