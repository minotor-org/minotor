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

#ifndef UIMASTER_H
#define UIMASTER_H

#include <QWidget>

#include "minomaster.h"
#include "uiprogramview.h"
#include "uianimationgroup.h"
#include "uianimation.h"
#include "uimastercontrol.h"

class UiMaster : public QWidget
{
    Q_OBJECT
public:
    explicit UiMaster(MinoMaster *master, QWidget *parent);
    UiAnimation* takeAnimationAt(int groupId, int animationId);
    UiAnimationGroup* findUiAnimationGroup(int groupId);
    void insertAnimation(UiAnimation *uiAnimation, int destGroupId, int destAnimationId);
signals:
    void midiLearnToggled(bool);
public slots:
    void updateProgram();
protected:
    MinoMaster *_master;
private:
    UiProgramView *_uiMasterMonitor;
    UiMasterControl *_uiMasterControl;
private slots:

};

#endif // UIMASTER_H
