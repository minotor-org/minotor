#ifndef UIPROGRAM_H
#define UIPROGRAM_H

#include <QWidget>
#include "minoprogram.h"

class UiProgram : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgram(MinoProgram *program, QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // UIPROGRAM_H
