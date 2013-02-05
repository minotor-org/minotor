#ifndef UICUE_H
#define UICUE_H

#include <QWidget>

#include "mipromatrix.h"

class UiCue : public QWidget
{
    Q_OBJECT
public:
    explicit UiCue(MinoProgram *program, QWidget *parent);
    
signals:
    
public slots:

private:
    MinoProgram *_program;
};

#endif // UICUE_H
