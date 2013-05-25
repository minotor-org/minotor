#ifndef MINOPROGRAM_H
#define MINOPROGRAM_H

#include <minopersistentobject.h>

#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QRect>

#include "minoanimation.h"
#include "minoanimationgroup.h"
#include "minoprogrambank.h"

class Minotor;

class MinoProgram : public MinoPersistentObject
{
    Q_OBJECT
    Q_PROPERTY (QString label READ label WRITE setLabel STORED true)

    // Friends can access to protected/private functions
    friend class MinoProgramBank;
    friend class MinoMaster;

public:
    explicit MinoProgram(QObject *parent);
    ~MinoProgram();

    void registerAnimationGroupEnableChange(MinoAnimationGroup *group, const bool on);

    // Accessors
    QGraphicsScene *scene() const { return _scene; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
    MinoAnimationGroupList animationGroups() const { return _animationGroups; }
    const QImage *rendering() const { return _image; }
    int id() const { return _id; }

    // Properties
    // FIXME: We may use a local property list instead of using findChildren()
    // MinoPropertyList _properties;
    // const MinoPropertyList properties() { return _properties; }

    // Selection
    bool isSelected() { return true; }
    bool isOnAir() { return _onAir; }

    // Function is compute height with a given width (very useful for UI)
    int heightForWidth( int width ) const { return (qreal)width * _heightForWidthRatio; }

    // Animation Group
    void addAnimationGroup(MinoAnimationGroup *group);
    void insertAnimationGroup(MinoAnimationGroup *group, int index);
    void moveAnimationGroup(int srcGroupId, int destGroupId, MinoProgram *destProgram = NULL);

    Minotor *minotor();
    QString label() const { return _label; }
    void setLabel(const QString& label) { _label = label; }

protected:
    // At end of object creation, Minotor will set ID and drawing rect
    void setId(const int id) { _id = id; }
    void setRect(const QRect rect);
    void setDrawingPos(const QPointF pos);

    // Acceded by MinoMaster
    void setOnAir(bool on);

private:
    // ID
    int _id;

    // Scene
    QGraphicsScene *_scene;

    // Drawing rect (in scene coordinates)
    QRect _rect;
    QPointF _drawingPos;

    // QImage to store rendering
    QImage *_image;

    // Image ratio
    qreal _heightForWidthRatio;

    // TODO: remove me and put me on master only
    MidiControllableList *_beatFactor;

    bool _onAir;

    QString _label;

protected:
    MinoAnimationGroupList _animationGroups;
    MinoAnimationGroupList _animationGroupsToEnable;
    MinoAnimationGroupList _animationGroupsToDisable;

    QGraphicsItemGroup _itemGroup;
    MinoAnimationGroup* takeAnimationGroupAt(int index);

signals:

    // Signal emitted when animate() is done
    void animated();

    // Signal emitted when "On Air" status changed (means Master uses a different program)
    void onAir(bool on);

    // Signal emitted when animations have been changed (ie. create, delete, moved)
    void updated();

    //Signal emitted when animation has been moved
    void animationMoved(int srcProgramId, int srcGroupId , int srcAnimationpId, int destProgramId, int destGroupId , int destAnimationpId);

    //Signal emitted when animation group has been moved
    void animationGroupMoved(QObject * group);

    // Signal emitted when a group is added to program
    void animationGroupAdded(QObject * group);

public:
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);

private slots:
    void destroyGroup(QObject *group);

};

#endif // MINOPROGRAM_H
