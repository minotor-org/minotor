#ifndef MINOPROGRAM_H
#define MINOPROGRAM_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>
#include <QRect>

#include "minoanimation.h"
#include "minoanimationgroup.h"

class Minotor;

class MinoProgram : public QObject
{
    Q_OBJECT

    // Minotor can access to protected/private functions
    friend class Minotor;
    friend class MinoMaster;

public:
    explicit MinoProgram(Minotor *minotor);
    ~MinoProgram();

    void registerAnimationGroupEnableChange(MinoAnimationGroup *group, const bool on);

    // Accessors
    QGraphicsScene *scene() const { return _scene; }
    QGraphicsItemGroup *itemGroup() { return &_itemGroup; }
    MinoAnimationGroupList animationGroups() const { return _animationGroups; }
    const QImage *rendering() const { return _image; }
    int id() const { return _id; }
    const MinoPropertyList properties() { return _properties; }

    // Selection
    bool isSelected() { return true; }
    bool isOnAir() { return _onAir; }

    // Function is compute height with a given width (very useful for UI)
    int heightForWidth( int width ) const { return (qreal)width * _heightForWidthRatio; }

    // Animation Group
    void addAnimationGroup(MinoAnimationGroup *group);
    void moveAnimation(MinoAnimationGroup *srcGroup, int srcAnimationId, MinoAnimationGroup *destGroup, int destAnimationId);
    void insertAnimationGroup(MinoAnimationGroup *animationGroup, int index);
    void moveAnimationGroup(int srcGroupId, int destGroupId);

    Minotor *minotor();

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

    // Properties
    MinoPropertyList _properties;
    MinoItemizedProperty _beatFactor;

    bool _onAir;

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
    void animationGroupMoved(int srcProgramId, int srcGroupId , int destGroupId);

public:
    void animate(const unsigned int uppqn, const unsigned int gppqn, const unsigned int ppqn, const unsigned int qn);
    void setUpdated();
private slots:
    void destroyGroup(QObject *group);

};

#endif // MINOPROGRAM_H
