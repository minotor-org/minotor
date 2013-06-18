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

#ifndef MINOANIMATIONGROUP_H
#define MINOANIMATIONGROUP_H

#include "minopersistentobject.h"

#include <QGraphicsItemGroup>
#include <QList>

#include "minoanimation.h"

class MinoProgram;

class MinoAnimationGroup : public MinoPersistentObject
{
    Q_OBJECT
    friend class MinoProgram;
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled STORED true)
    Q_PROPERTY(QPixmap screenshot READ screenshot WRITE setScreenshot STORED true)
public:
    explicit MinoAnimationGroup(QObject *parent);

    ~MinoAnimationGroup();

    // Accessors
    int id();
    QPixmap screenshot() const { return _screenshot; }
    MinoAnimationList animations() const { return _animations; }
    void setScreenshot(const QPixmap &screenshot);

    void setProgram(MinoProgram *program);
    MinoProgram *program() const { return _program; }

    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }

    bool enabled() const { return _enabled; }
    bool isAlive() const { return _alive; }
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

    // Animation manipulation
    void addAnimation(MinoAnimation *animation);
    MinoAnimation* addAnimation(const QString &animationClassName, int index = -1);

    void moveAnimation(int srcIndex, int destIndex, MinoAnimationGroup *destGroup = NULL);
    MinoAnimation* takeAnimationAt(int index);
    void insertAnimation(MinoAnimation *animation, int index = -1);

public slots:
    void setDelayedEnabled(const bool enabled);
    void setEnabled(bool on);
    void toogle() { _setEnabled(!_enabled); }

private:
    QPixmap _screenshot;
    MinoAnimationList _animations;
    bool _enabled;
    bool _alive;
    MinoProgram *_program;
    QGraphicsItemGroup _itemGroup;

    // Will be called by MinoProgram
    void _setEnabled(const bool on);
    void reorderAnimations();

private slots:
    void destroyAnimation(QObject *animation);

signals:
    // Signal emitted when group is enabled
    void enabledChanged(bool on);
    void animationAdded(QObject *animation);
    void animationMoved(QObject *animation);
    void screenshotUpdated();
};

typedef QList<MinoAnimationGroup*> MinoAnimationGroupList;

#endif // MINOANIMATIONGROUP_H
