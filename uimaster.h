#ifndef UIMASTER_H
#define UIMASTER_H

#include <QWidget>
#include "minotor.h"

class UiMaster : public QWidget
{
    Q_OBJECT
public:
    explicit UiMaster(Minotor *minotor, QWidget *parent);
    
signals:

public slots:
    
};

#endif // UIMASTER_H
