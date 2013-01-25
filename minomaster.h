#ifndef MINOMASTER_H
#define MINOMASTER_H

#include "minochannel.h"

class Minotor;

class MinoMaster : public MinoChannel
{
    Q_OBJECT
public:
    explicit MinoMaster(Minotor *minotor);
    ~MinoMaster();

    QString name() { return QString("Master"); }
private:
signals:
    
public slots:
    void setBrightness(qreal value);
};

#endif // MINOMASTER_H
