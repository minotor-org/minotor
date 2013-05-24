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

    void setViewportRange(const int min, const int max);

private:
    MinoProgram *_program;
    bool _shifted;
    QGraphicsItemGroup _itemGroup;

signals:
    void programChanged();
    void viewportRangeChanged(const int min, const int max);
    void updated();

public slots:
    void setBrightness(qreal value);
    void clear();
};

#endif // MINOMASTER_H
