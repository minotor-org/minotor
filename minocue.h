#ifndef MINOCUE_H
#define MINOCUE_H

#include "minochannel.h"

class Minotor;

class MinoCue : public MinoChannel
{
    Q_OBJECT
public:
    explicit MinoCue(Minotor *minotor);
    QString name() { return QString("Cue"); }

signals:
    
public slots:
    
};

#endif // MINOCUE_H
