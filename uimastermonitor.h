#ifndef MONITOR_H
#define MONITOR_H

#include <QPaintEvent>
#include <QWidget>

#include "ledmatrix.h"

class UiMasterMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit UiMasterMonitor(LedMatrix *ledMatrix, QWidget *parent = 0);
protected:
        void paintEvent(QPaintEvent *event);
        int heightForWidth( int width ) const;
private:
        LedMatrix *_ledMatrix;

signals:
    
public slots:
    
};

#endif // MONITOR_H
