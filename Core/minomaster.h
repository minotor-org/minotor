#ifndef MINOMASTER_H
#define MINOMASTER_H

#include "minoprogram.h"
#include "minomastermidimapper.h"

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
    MinoMasterMidiMapper *_midiMapper;

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
