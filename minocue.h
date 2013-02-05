#ifndef MINOCUE_H
#define MINOCUE_H

#include "minoprogram.h"

class MinoCue : public MinoProgram
{
    Q_OBJECT
public:
    explicit MinoCue(Minotor *minotor);
    QString name() { return QString("Cue"); }

signals:
    
public slots:
    
};

#endif // MINOCUE_H
