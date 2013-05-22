#ifndef UIPROGRAMBANK_H
#define UIPROGRAMBANK_H

#include <QWidget>
#include <QButtonGroup>
#include "minoprogrambank.h"

class UiProgramBank : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramBank(MinoProgramBank *bank, QWidget *parent = 0);
    
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
