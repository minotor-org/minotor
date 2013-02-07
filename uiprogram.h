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
    MinoProgram *_program;
    QFrame *_fPreview;
    QWidget *_wBackground;
    QWidget *_border;
signals:

public slots:
    void setExpanded(bool expanded);
    void setOnAir(bool onAir);
};

#endif // UIPROGRAM_H
