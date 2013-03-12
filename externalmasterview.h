#ifndef EXTERNALMASTERVIEW_H
#define EXTERNALMASTERVIEW_H

#include <QDialog>
#include "uiprogramview.h"

namespace Ui {
class ExternalMasterView;
}

class ExternalMasterView : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExternalMasterView(QWidget *parent = 0);
    ~ExternalMasterView();
    
private:
    Ui::ExternalMasterView *ui;
    UiProgramView *_uiMasterMonitor;
private slots:
    void updateProgram();
    void mousePressEvent(QMouseEvent *event);
    void tbFullScreenToggled(bool on);
    void keyPressEvent(QKeyEvent *e);
};

#endif // EXTERNALMASTERVIEW_H
