#ifndef UIMASTER_H
#define UIMASTER_H

#include <QWidget>
#include "minotor.h"

class UiMaster : public QWidget
{
    Q_OBJECT
public:
    explicit UiMaster(MinoMaster *master, QWidget *parent);
    
signals:

private slots:
    void brightnessChanged(int value);

protected:
    MinoMaster *_master;
};

#endif // UIMASTER_H
