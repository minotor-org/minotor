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
    void programSelectorChanged(QObject *program);

private:
    // Programs
    QList<MinoProgram*> _programs;
    int _programSelectorPos;
    
public slots:

protected slots:
    void destroyProgram(QObject *program);

    void programSelectorNext();
    void programSelectorPrevious();
    void programSelectorSelect();
};

#endif // MINOPROGRAMBANK_H
