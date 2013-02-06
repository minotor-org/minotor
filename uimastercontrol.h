#ifndef UIMASTERCONTROL_H
#define UIMASTERCONTROL_H

#include <QWidget>

#include <QDragEnterEvent>
#include <QDropEvent>

#include "minomaster.h"

class UiMasterControl : public QWidget
{
    Q_OBJECT
public:
    explicit UiMasterControl(MinoMaster *master, QWidget *parent);
    ~UiMasterControl();

protected:
    QWidget *_wContent;

    void addAnimation(MinoAnimation *animation);
signals:
    
public slots:

private slots:


private:
    MinoMaster *_master;
};

#endif // UIMASTERCONTROL_H
