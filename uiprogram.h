#ifndef UIPROGRAM_H
#define UIPROGRAM_H

#include <QWidget>
#include <QFrame>
#include "minoprogram.h"

class UiProgram : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgram(MinoProgram *program, QWidget *parent = 0);
private:
    QFrame *_fPreview;
    QWidget *_wBackground;
signals:
    
public slots:
    void setExpanded(bool expanded);
};

#endif // UIPROGRAM_H
