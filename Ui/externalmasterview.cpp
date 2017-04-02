/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "externalmasterview.h"
#include "ui_externalmasterview.h"
#include "minotor.h"

#include <QLayout>
#include <QMouseEvent>
#include <QDebug>

ExternalMasterView::ExternalMasterView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExternalMasterView)
{
    QVBoxLayout *lMain = new QVBoxLayout(this);
    lMain->setSpacing(0);
    lMain->setMargin(0);
    lMain->setContentsMargins(0,0,0,0);
    connect(Minotor::minotor()->master(),SIGNAL(programChanged()),this,SLOT(updateProgram()));
    _uiMasterMonitor = new UiProgramView(Minotor::minotor()->master()->program(), this);
    _uiMasterMonitor->setObjectName("masterMonitor");

    ui->setupUi(this);
    this->layout()->addWidget(_uiMasterMonitor);
    this->setWindowFlags(this->windowFlags() | Qt::Window | Qt::CustomizeWindowHint | Qt::FramelessWindowHint ); //Qt::FramelessWindowHint for disable border
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
