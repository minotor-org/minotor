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

#ifndef UIMASTERANIMATIONGROUP_H
#define UIMASTERANIMATIONGROUP_H

#include <QGroupBox>
#include <QPushButton>
#include <QLayout>

#include "minoanimationgroup.h"
#include "minoanimation.h"
#include "uianimation.h"

class UiMasterAnimationGroup : public QGroupBox
{
    Q_OBJECT
public:
    explicit UiMasterAnimationGroup(MinoAnimationGroup *group, QWidget *parent);
    ~UiMasterAnimationGroup();

    void setHighlight(const bool on);

    void addAnimation(MinoAnimation *animation, int index = -1);
    UiAnimation *takeAt(int index);
    void moveAnimation(int srcId, int destId);
    void insertAnimation(UiAnimation *animation, int destId);

    // Accessors
    MinoAnimationGroup* group() const { return _group; }

private:
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    MinoAnimationGroup *_group;
    QWidget *_wHighlight;
    QWidget *_wContent;
    QWidget *_wGroupParameters;

    // Flagged as "Important" parameters
    QBoxLayout *_lImportantParameters;
    QWidget *_wImportantParameters;

    // MIDI mapped parameters
    QBoxLayout *_lMidiParameters;
    QWidget *_wMidiParameters;

    QPushButton *_pbEnable;
    bool _highlight;

signals:
    
public slots:
    void enable(bool on);
    void updateGroup();

private slots:
    void reloadScreenshot();
};

#endif // UIMASTERANIMATIONGROUP_H
