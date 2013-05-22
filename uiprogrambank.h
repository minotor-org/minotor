#ifndef UIPROGRAMBANK_H
#define UIPROGRAMBANK_H

#include <QWidget>
#include <QButtonGroup>
#include "minotor.h"

class UiProgramBank : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramBank(Minotor *minotor,QWidget *parent = 0);
    
signals:
    
public slots:
private:
    QWidget *_wScrollContent;
    QButtonGroup *_bgOnAir;

    void addProgram(MinoProgram *program);

private slots:
    void addProgram(QObject *program);
};

#endif // UIPROGRAMBANK_H
