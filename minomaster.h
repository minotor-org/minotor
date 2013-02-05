#ifndef MINOMASTER_H
#define MINOMASTER_H

#include "minoprogram.h"

class MinoMaster : public QObject
{
    Q_OBJECT
public:
    explicit MinoMaster();
    ~MinoMaster();

    QString name() { return QString("Master"); }
    void setProgram(MinoProgram *program) { _program = program; }
    MinoProgram *program() { return _program; }
private:
    MinoProgram *_program;
signals:
    
public slots:
    void setBrightness(qreal value);
};

#endif // MINOMASTER_H
