#include "externalmasterview.h"
#include "ui_externalmasterview.h"
#include "minotor.h"

#include <QMouseEvent>
#include <QDebug>

ExternalMasterView::ExternalMasterView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExternalMasterView)
{
    connect(Minotor::minotor()->master(),SIGNAL(programChanged()),this,SLOT(updateProgram()));
    _uiMasterMonitor = new UiProgramView(Minotor::minotor()->master()->program(), this);
    _uiMasterMonitor->setObjectName("masterMonitor");

    ui->setupUi(this);
    this->layout()->addWidget(_uiMasterMonitor);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
}

void ExternalMasterView::updateProgram()
{
    _uiMasterMonitor->setProgram(Minotor::minotor()->master()->program());
}

ExternalMasterView::~ExternalMasterView()
{
    delete ui;
}

void ExternalMasterView::mousePressEvent(QMouseEvent *event)
{
    if (event->type() ==  QEvent::MouseButtonDblClick)
    {
        tbFullScreenToggled(false);
    }
}

void ExternalMasterView::tbFullScreenToggled(bool on)
{
    (void)on;
    if(isFullScreen()) {
         this->setWindowState(Qt::WindowMaximized);
      } else {
         this->setWindowState(Qt::WindowFullScreen);
      }
}

void ExternalMasterView::keyPressEvent(QKeyEvent *e) {
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
    else {
        tbFullScreenToggled(false);
    }
}
