#ifndef UIMASTER_H
#define UIMASTER_H

#include <QWidget>

#include "minomaster.h"
#include "uiprogramview.h"

class UiMaster : public QWidget
{
    Q_OBJECT
public:
    explicit UiMaster(MinoMaster *master, QWidget *parent);
    
signals:
    void midiLearnToggled(bool);
private slots:
    void updateProgram();
protected:
    MinoMaster *_master;
private:
    UiProgramView *_uiMasterMonitor;
};

#endif // UIMASTER_H
