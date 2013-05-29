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
