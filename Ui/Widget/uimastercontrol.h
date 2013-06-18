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

#ifndef UIMASTERCONTROL_H
#define UIMASTERCONTROL_H

#include <QWidget>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QScrollArea>

#include "minomaster.h"
#include "uimasteranimationgroup.h"

class UiMasterControl : public QWidget
{
    Q_OBJECT
public:
    explicit UiMasterControl(MinoMaster *master, QWidget *parent);
    ~UiMasterControl();

protected:
    QWidget *_wContent;

    void addAnimationGroup(MinoAnimationGroup *group);
    UiMasterAnimationGroup* takeAnimationGroupAt(int groupId);
    void insertAnimationGroup(UiMasterAnimationGroup *uiMasterAnimationGroup, int destGroupId);

signals:
    
public slots:
    void updateProgram();

private slots:
    void animationMoved(int srcProgramId, int srcGroupId , int srcAnimationId, int destProgramId, int destGroupId , int destAnimationId);
    void animationGroupMoved(int srcProgramId, int srcGroupId, int destGroupId);
    void changeViewportRange(const int min, const int max);

private:
    MinoMaster *_master;
    int _viewportMin;
    int _viewportMax;
    QScrollArea *_sa;
};

#endif // UIMASTERCONTROL_H
