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

#ifndef UIPROGRAMEDITOR_H
#define UIPROGRAMEDITOR_H

#include <QWidget>
#include <QLayout>

#include <QDragEnterEvent>
#include <QDropEvent>

#include "minoprogram.h"
#include "minoanimationgroup.h"
#include "uianimationgroup.h"

class UiProgramEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiProgramEditor(MinoProgram *program, QWidget *parent);
    ~UiProgramEditor();
    UiAnimation* takeAnimationAt(int groupId, int animationId);
    UiAnimationGroup* findUiAnimationGroup(int groupId);
    UiAnimationGroup* takeAnimationGroupAt(int groupId);
    void insertUiAnimationGroup(UiAnimationGroup *uiAnimationGroup, int destGroupId);

    template<typename T> T findParent();
    template<typename T> T findContainer(QWidget* parent, const QPoint pos);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

    bool dropMinoAnimationDescription(QDropEvent *event);
    bool dropMinoAnimation(QDropEvent *event);
    bool dropMinoAnimationGroup(QDropEvent *event);

    QWidget *_wContent;
    QBoxLayout *_lContent;

    UiAnimationGroup* addAnimationGroup(MinoAnimationGroup *group);
    void requestAnimationMove(MinoAnimation *srcAnimation, MinoAnimationGroup *destGroup, int destAnimationId);

signals:

public slots:
    void setExpanded(bool expanded);

private slots:
    void addAnimationGroup(QObject* group);
    void moveAnimationGroup(QObject *group);

private:
    MinoProgram *_program;
    bool _expanded;
};

#endif // UIPROGRAMEDITOR_H
