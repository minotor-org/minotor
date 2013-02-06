#ifndef MIPROMATRIX_H
#define MIPROMATRIX_H

#include "minoprogram.h"

class MiproMatrix : public MinoProgram
{
    Q_OBJECT
public:
    explicit MiproMatrix(Minotor *minotor);
    QString name() { return QString("Matrix"); }

signals:
    
public slots:
    
};

#endif // MIPROMATRIX_H
