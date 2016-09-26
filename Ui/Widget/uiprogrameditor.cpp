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

#include "uiprogrameditor.h"

#include "minoanimation.h"
#include "uiprogramview.h"
#include <QFrame>
#include <QScrollArea>
#include <QLayout>
#include <QStyle>
#include <QMimeData>

#include <QDebug>

#include "uianimation.h"
#include "uianimationgroup.h"
#include "uiprogrambank.h"

UiProgramEditor::UiProgramEditor(MinoProgram *program, QWidget *parent) :
    QWidget(parent),
    _program(program),
    _expanded(true)
{
    QString objectName = QString("Ui:%1(%2)")
            .arg(program->metaObject()->className())
            .arg(QString::number((ulong)((void*)program), 16));
    setObjectName(objectName);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QWidget *wContainer = new QWidget(this);
    layout->addWidget(wContainer);
    wContainer->setObjectName("scroll");
    QVBoxLayout *lContainer = new QVBoxLayout(wContainer);
    lContainer->setSpacing(0);
    lContainer->setMargin(0);
    lContainer->setContentsMargins(2,2,2,2);

    QWidget *wContent = new QWidget(wContainer);
    lContainer->addWidget(wContent);
    wContent->setObjectName("scrollcontent");
    QVBoxLayout *lContent = new QVBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(3,3,3,3);
    // Animations properties
    QScrollArea *sa = new QScrollArea(this);
    lContent->addWidget(sa);

    _wContent = new QWidget(this);
    _wContent->setObjectName("scrollbackground");
    sa->setWidget(_wContent);
    sa->setFrameShadow(QFrame::Plain);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setWidgetResizable(true);

    _lContent = new QHBoxLayout(_wContent);
    _lContent->addStretch(1);
    _lContent->setContentsMargins(5,5,50,5);

    foreach (MinoAnimationGroup *group, _program->animationGroups())
    {
        addAnimationGroup(group);
    }

    connect(_program, SIGNAL(animationGroupAdded(QObject*)), this, SLOT(addAnimationGroup(QObject*)));
    connect(_program, SIGNAL(animationGroupMoved(QObject*)), this, SLOT(moveAnimationGroup(QObject*)));

    setAcceptDrops(true);
}

template<typename T>
T UiProgramEditor::findParent()
{
    QObject *p = this;
    while((p = p->parent()))
    {
        T r = qobject_cast<T>(p);
        if(r)
            return r;
    }
    return NULL;
}

UiAnimationGroup* UiProgramEditor::addAnimationGroup(MinoAnimationGroup *group)
{
    UiAnimationGroup *uiAnimationGroup = new UiAnimationGroup(group, _wContent);
    uiAnimationGroup->setExpanded(_expanded);

    _lContent->insertWidget(_lContent->count()-1, uiAnimationGroup);
    return uiAnimationGroup;
}

void UiProgramEditor::addAnimationGroup(QObject *group)
{
    qDebug() << Q_FUNC_INFO;
    MinoAnimationGroup *g = qobject_cast<MinoAnimationGroup*>(group);
    Q_ASSERT(g);
    addAnimationGroup(g);
}

void UiProgramEditor::moveAnimationGroup(QObject *group)
{
    qDebug() << Q_FUNC_INFO
             << "group:" << group
             << "this:" << this;

    // Find program bank from this
    UiProgramBank* uipb = findParent<UiProgramBank*>();
    Q_ASSERT(uipb);

    // Look for corresponding uianimation into bank
    QString objectName = QString("Ui:%1(%2)")
            .arg(group->metaObject()->className())
            .arg(QString::number((ulong)((void*)group), 16));
    QList<UiAnimationGroup*> uiagl = findChildren<UiAnimationGroup*>(objectName);
    Q_ASSERT(uiagl.count()==1);
    UiAnimationGroup *uiag = uiagl.at(0);

    // Remove uigroup from this uiprogrameditor
    _lContent->removeWidget(uiag);

    MinoAnimationGroup* mag = qobject_cast<MinoAnimationGroup*>(group);
    Q_ASSERT(mag);

    if(_program==mag->program())
    {
        // Group moved within this program
        insertUiAnimationGroup(uiag, uiag->group()->id());
    }
    else
    {
        // Look for corresponding uiprogrameditor into bank
        QString objectName = QString("Ui:%1(%2)")
                .arg(mag->program()->metaObject()->className())
                .arg(QString::number((ulong)((void*)mag->program()), 16));
        QList<UiProgramEditor*> uipel = uipb->findChildren<UiProgramEditor*>(objectName);
        qDebug() << "uipel.count()" << uipel.count();
        Q_ASSERT(uipel.count()==1);
        UiProgramEditor *uipe = uipel.at(0);
        Q_ASSERT(uipe);
        Q_ASSERT(uipe != this);
        uipe->insertUiAnimationGroup(uiag, uiag->group()->id());
    }
}

UiProgramEditor::~UiProgramEditor()
{
}

void UiProgramEditor::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    if (event->mimeData()->hasFormat("application/x-dnd_minoanimationdescription")) {
        event->accept();
    } else if (event->mimeData()->hasFormat("application/x-dnd_minoanimation")) {
        event->accept();
    } else if (event->mimeData()->hasFormat("application/x-dnd_minoanimationgroup")) {
        event->accept();
    } else {
        qDebug() << Q_FUNC_INFO
                 << "ERROR: This should not happends !!!";
        Q_ASSERT(false);
        event->ignore();
    }
    event->accept();
}

void UiProgramEditor::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnd_minoanimationdescription")) {
        event->accept();
    } else if (event->mimeData()->hasFormat("application/x-dnd_minoanimation")) {
        event->accept();
    } else if (event->mimeData()->hasFormat("application/x-dnd_minoanimationgroup")) {
        event->accept();
    } else {
        qDebug() << Q_FUNC_INFO
                 << "ERROR: This should not happends !!!";
        Q_ASSERT(false);
        event->ignore();
    }
    event->accept();
}

void UiProgramEditor::dropEvent(QDropEvent *event)
{
    if (dropMinoAnimationDescription(event)) {
        qDebug() << Q_FUNC_INFO
                 << "MinoAnimationDescription successfully dropped";
    } else if (dropMinoAnimation(event)) {
        qDebug() << Q_FUNC_INFO
                 << "MinoAnimation successfully dropped";
    }  else if (dropMinoAnimationGroup(event)) {
        qDebug() << Q_FUNC_INFO
                 << "MinoAnimationGroup successfully dropped";
    }
    event->accept();
}

template<typename T>
T UiProgramEditor::findContainer(QWidget* parent, const QPoint pos)
{
    T container = NULL;
    QList<T> children = parent->findChildren<T>();
    for (int i=0;i<children.count();i++)
    {
        T child = children.at(i);
        if (child->rect().contains(child->mapFrom(this,pos)))
        {
            container = child;
            break;
        }
    }
    return container;
}

bool UiProgramEditor::dropMinoAnimationDescription(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat("application/x-dnd_minoanimationdescription"))
    {
        return false;
    }

    QByteArray itemData = event->mimeData()->data("application/x-dnd_minoanimationdescription");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString className;
    QPoint offset;
    dataStream >> className >> offset;
    qDebug() << Q_FUNC_INFO
             << "source className:" << className;

    // Find destination
    UiAnimationGroup* destUiGroup = findContainer<UiAnimationGroup*>(_wContent, event->pos());
    MinoAnimationGroup* destGroup = NULL;

    int destAnimationId = -1;
    if(destUiGroup) {
        // Destination group have been located
        destGroup = destUiGroup->group();

        // If possible, locate the target animation (ie. to retrieve position)
        UiAnimation* destUiAnimation = findContainer<UiAnimation*>(destUiGroup, event->pos());
        if(destUiAnimation)
        {
            qDebug() << destUiAnimation;
            destAnimationId = destUiAnimation->animation()->id();
        }
    } else {
        // No UiAnimationGroup have not been found: we assume user drop on UiProgramEditor widget and expect to place its animation at the end of program
        destGroup = new MinoAnimationGroup(_program);
        _program->addAnimationGroup(destGroup);
    }

    // Request animation add
    destGroup->addAnimation(className,destAnimationId);
    return true;
}

bool UiProgramEditor::dropMinoAnimation(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat("application/x-dnd_minoanimation"))
    {
        return false;
    }

    QByteArray itemData = event->mimeData()->data("application/x-dnd_minoanimation");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString objectName;
    QPoint offset;

    dataStream >> objectName >> offset;
    qDebug() << Q_FUNC_INFO
             << "source objectName:" << objectName;

    // Find program bank from this
    UiProgramBank* uipb = findParent<UiProgramBank*>();
    Q_ASSERT(uipb);

    // Find source object
    QList<UiAnimation*> srcUiAnimations = uipb->findChildren<UiAnimation*>(objectName);
    Q_ASSERT(srcUiAnimations.count()==1);
    MinoAnimation *srcAnimation = srcUiAnimations.at(0)->animation();
    Q_ASSERT(srcAnimation);

    // Find destination
    UiAnimationGroup* destUiGroup = findContainer<UiAnimationGroup*>(_wContent, event->pos());
    MinoAnimationGroup* destGroup = NULL;

    int destAnimationId = -1;
    if(destUiGroup) {
        // Destination group have been located
        destGroup = destUiGroup->group();

        // If possible, locate the target animation (ie. to retrieve position)
        UiAnimation* destUiAnimation = findContainer<UiAnimation*>(destUiGroup, event->pos());
        if(destUiAnimation)
        {
            qDebug() << destUiAnimation;
            destAnimationId = destUiAnimation->animation()->id();
        }
    } else {
        // No UiAnimationGroup have not been found: we assume user drop on UiProgramEditor widget and expect to place its animation at the end of program
        destGroup = new MinoAnimationGroup(_program);
        _program->addAnimationGroup(destGroup);
    }

    // Request animation move to engine
    requestAnimationMove(srcAnimation, destGroup, destAnimationId);
    return true;
}

bool UiProgramEditor::dropMinoAnimationGroup(QDropEvent *event)
{
    if(!event->mimeData()->hasFormat("application/x-dnd_minoanimationgroup"))
    {
        return false;
    }
    QByteArray itemData = event->mimeData()->data("application/x-dnd_minoanimationgroup");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QString objectName;
    QPoint offset;

    dataStream >> objectName >> offset;
    qDebug() << Q_FUNC_INFO
             << " source objectName:" << objectName;

    // Find program bank from this
    UiProgramBank* uipb = findParent<UiProgramBank*>();
    Q_ASSERT(uipb);

    // Find source object
    QList<UiAnimationGroup*> srcUiGroups = uipb->findChildren<UiAnimationGroup*>(objectName);
    Q_ASSERT(srcUiGroups.count()==1);
    MinoAnimationGroup *srcGroup = srcUiGroups.at(0)->group();
    Q_ASSERT(srcGroup);

    // Find destination
    UiAnimationGroup* destUiGroup = findContainer<UiAnimationGroup*>(_wContent, event->pos());

    int destGroupId = -1;
    if(destUiGroup) {
        // Destination group have been located
        destGroupId = destUiGroup->group()->id();
    }

    // Request animation move to engine
    uipb->requestAnimationGroupMove(srcGroup, _program, destGroupId);
    return true;
}

void UiProgramEditor::setExpanded(bool expanded)
{
    _expanded = expanded;
    QList<UiAnimationGroup*> uiGroups = _wContent->findChildren<UiAnimationGroup*>();
    foreach(UiAnimationGroup *uiGroup, uiGroups)
    {
        uiGroup->setExpanded(expanded);
    }
}

UiAnimationGroup* UiProgramEditor::findUiAnimationGroup(int groupId)
{
    UiAnimationGroup *uiAnimationGroup = NULL;

    qDebug() << Q_FUNC_INFO
             << "groupId:" << groupId;
    QList<UiAnimationGroup*> uiAnimationGroups = this->findChildren<UiAnimationGroup*>();

    for (int j=0; j<uiAnimationGroups.count(); j++)
    {
        int currentGroupId = uiAnimationGroups.at(j)->group()->id();
        if(currentGroupId == groupId)
        {
            uiAnimationGroup = uiAnimationGroups.at(j);
            break;
        }
    }
    qDebug() << Q_FUNC_INFO
             << "found:" << uiAnimationGroup;
    return uiAnimationGroup;
}

UiAnimationGroup* UiProgramEditor::takeAnimationGroupAt(int groupId)
{
    UiAnimationGroup *ret = NULL;
    QList<UiAnimationGroup*> uiAnimationGroups = this->findChildren<UiAnimationGroup*>();
    if(groupId == -1)
    {
        groupId=uiAnimationGroups.count()-1;
    }
    if(uiAnimationGroups.count()>0)
    {
        qDebug() << Q_FUNC_INFO
                 << "program has" << uiAnimationGroups.count() << "items";
        for (int j=0; j<uiAnimationGroups.count(); j++)
        {
            if(uiAnimationGroups.at(j)->group()->id() == groupId)
            {
                ret = uiAnimationGroups.takeAt(j);
                break;
            }
        }
    }
    return ret;
}

void UiProgramEditor::insertUiAnimationGroup(UiAnimationGroup *uiAnimationGroup, int destGroupId)
{
    if (destGroupId == -1)
    {
        //Place is at the end of the list (-1 due to a spacer placed at the end)
        destGroupId =  _lContent->count()-2;
    }
    _lContent->insertWidget(destGroupId, uiAnimationGroup);
    uiAnimationGroup->setParent(_wContent);
    uiAnimationGroup->setExpanded(_expanded);
    uiAnimationGroup->show();
}

void UiProgramEditor::requestAnimationMove(MinoAnimation *srcAnimation, MinoAnimationGroup *destGroup, int destAnimationId)
{
    qDebug() << Q_FUNC_INFO
             << "srcAnimation:" << srcAnimation
             << "destGroup:" << destGroup
             << "destAnimationId" << destAnimationId;
    MinoAnimationGroup *srcGroup = srcAnimation->group();
    Q_ASSERT(srcGroup);
    Q_ASSERT(destGroup);
    srcGroup->moveAnimation(srcAnimation->id(), destAnimationId, destGroup);
}
