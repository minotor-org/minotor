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

#ifndef UIANIMATIONGROUP_H
#define UIANIMATIONGROUP_H

#include <QGroupBox>
#include <QPushButton>
#include <QLayout>
#include <QMouseEvent>

#include "minoanimationgroup.h"
#include "minoanimation.h"
#include "uianimation.h"

class UiAnimationGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiAnimationGroup(MinoAnimationGroup *group, QWidget *parent);
    ~UiAnimationGroup();

    void setExpanded(bool on);
    void addAnimation(MinoAnimation *animation, int index = -1);
    void insertUiAnimation(UiAnimation *animation, int destId);
    // Accessors
    MinoAnimationGroup* group() const { return _group; }

    template<typename T> T findParent();
private:
    MinoAnimationGroup *_group;
    QWidget *_wContent;
    QHBoxLayout *_lAnimations;
    QWidget *_wAnimations;
    QPushButton *_pbEnable;
    bool _expanded;
signals:
    void animationMoved(QObject* uiAnimation, int programId, int groupId);
public slots:
    void enable(bool on);
    
protected slots:
    void mousePressEvent(QMouseEvent *event);
    void takeAShot();
private slots:
    void addAnimation(QObject *animation);
    void moveAnimation(QObject *animation);

};

#endif // UIANIMATIONGROUP_H
