#ifndef MONITOR_H
#define MONITOR_H

#include <QPaintEvent>
#include <QWidget>

#include "ledmatrix.h"

class UiMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit UiMonitor(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);
    void setLedMatrix(LedMatrix *matrix);
    private:
        LedMatrix *_ledMatrix;
    signals:
    
public slots:
    
};

#endif // MONITOR_H
