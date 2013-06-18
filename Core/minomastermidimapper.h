#ifndef MINOMASTERMIDIMAPPER_H
#define MINOMASTERMIDIMAPPER_H

#include <QObject>

class MinoMaster;
class MinoProgram;
class MinoAnimationGroup;
class MinoAnimation;

class MinoMasterMidiMapper : public QObject
{
    Q_OBJECT
public:
    explicit MinoMasterMidiMapper(MinoMaster *parent);

signals:
    void updated();

public slots:
    void updateProgram();

protected slots:
    void addGroup(QObject *group);
    void moveGroup(QObject *group);

    void addAnimation(QObject *animation);
    void moveAnimation(QObject *animation);

    void changeMidiControllableParameterAttributes();

protected:
    MinoMaster *_master;
    MinoProgram *_program;

    void registerProgram(MinoProgram *mp);
    void unregisterProgram(MinoProgram *mp);

    void registerGroup(MinoAnimationGroup *mag);
    void unregisterGroup(MinoAnimationGroup *mag);

    void registerAnimation(MinoAnimation *ma);
    void unregisterAnimation(MinoAnimation *ma);

    void registerRoles();
    void clearRoles();

    void updateMap();
};

#endif // MINOMASTERMIDIMAPPER_H
