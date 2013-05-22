#ifndef MINOPROGRAMBANK_H
#define MINOPROGRAMBANK_H

#include <QList>
#include "minopersistentobject.h"
#include "minoprogram.h"
class Minotor;

class MinoProgramBank : public MinoPersistentObject
{
    Q_OBJECT
public:
    explicit MinoProgramBank(QObject *parent);
    // Program
    void addProgram(MinoProgram *program);
    QList<MinoProgram*> programs() { return _programs; }
    ~MinoProgramBank();
    Minotor *minotor();
    
signals:
    void programAdded(QObject *program);

private:
    // Programs
    QList<MinoProgram*> _programs;
    
public slots:
    
};

#endif // MINOPROGRAMBANK_H
