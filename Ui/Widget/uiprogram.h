#ifndef UIPROGRAM_H
#define UIPROGRAM_H

#include <QWidget>
#include <QFrame>
#include "minoprogram.h"

class UiProgram : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgram(MinoProgram *program, QWidget *parent);
    void setOnAir(bool onAir);
    void setHighlight(bool on);
    MinoProgram *program() { return _program; }

private:
    MinoProgram *_program;
    QFrame *_fPreview;
    QWidget *_wBackground;
    QWidget *_wBorder;
    bool _highlight;
    QWidget *_wHighlight;

signals:

public slots:
    void setExpanded(bool expanded);
private slots:
    void updateOnAirStatus(bool onAir);
    void requestMasterProgramChange(bool on);
    void exportProgram();
};

#endif // UIPROGRAM_H
