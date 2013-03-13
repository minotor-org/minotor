#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QStyle>
#include <QMenu>
#include <QCheckBox>
#include <QToolButton>
#include <QMouseEvent>
#include <QPainter>

#include <QDebug>

#include "uianimationproperty.h"

#include "minoprogram.h"
#include "minoanimationgroup.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QGroupBox(parent),
    _animation(animation)
{
    this->setMinimumWidth(100);
    this->setMaximumWidth(100);

    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    lGroupBox->setSpacing(0);
    lGroupBox->setMargin(0);
    lGroupBox->setContentsMargins(1,1,1,1);
    QWidget *wContent = new QWidget(this);
    wContent->setObjectName("content");
    lGroupBox->addWidget(wContent);
    QVBoxLayout *lContent = new QVBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);

    QWidget *wDescription = new QWidget(this);
    lContent->addWidget(wDescription);
    QHBoxLayout *lDescription = new QHBoxLayout(wDescription);
    lDescription->setSpacing(0);
    lDescription->setMargin(0);
    lDescription->setContentsMargins(0,3,0,0);
    lDescription->addStretch();
    MinoAnimationDescription desc = animation->description();
    _tAnimation = new QLabel(desc.name());
    if(!desc.pixmap().isNull())
        _tAnimation->setPixmap(desc.pixmap().scaled(60,40,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    _tAnimation->setObjectName("image");
    _tAnimation->setAlignment(Qt::AlignCenter);
    _tAnimation->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    _tAnimation->setMinimumSize(60,40);
    _tAnimation->setMaximumSize(60,40);

    lDescription->addWidget(_tAnimation);
    lDescription->addStretch();

    _wProperties = new QWidget(wContent);
    QVBoxLayout *lProperties = new QVBoxLayout(_wProperties);
    lContent->addWidget(_wProperties);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    for (int i=0;i<animation->propertyGrouped().count();i++)
    {
        MinoPropertyList *group = animation->propertyGrouped().at(i);
        if (i>0)
        {
            QFrame *fSeparator = new QFrame(_wProperties);
            fSeparator->setObjectName("line");
            fSeparator->setFrameShape(QFrame::HLine);
            fSeparator->setFrameShadow(QFrame::Sunken);
            fSeparator->setLineWidth(1);
            lProperties->addWidget(fSeparator);
        }
        QWidget *wPropGroup = new QWidget(_wProperties);
        lProperties->addWidget(wPropGroup);
        QHBoxLayout *lPropGroup = new QHBoxLayout(wPropGroup);
        lPropGroup->setSpacing(0);
        lPropGroup->setMargin(0);
        lPropGroup->setContentsMargins(0,0,0,0);

        for (int i=0;i<group->length();i++)
        {
            MinoProperty *property = group->at(i);
            UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wProperties);
            uiAnimationProperty->setObjectName("animationproperty");
            lPropGroup->addWidget(uiAnimationProperty);
        }
    }
    lProperties->addStretch();
    lContent->addStretch();
    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));
    connect(animation, SIGNAL(groupChanged(int,int)), this, SLOT(changeAnimationGroup(int,int)));

    this->setAttribute(Qt::WA_NoMousePropagation, true);
}

void UiAnimation::setExpanded(bool expanded)
{
    _wProperties->setVisible(expanded);
}

void UiAnimation::mousePressEvent(QMouseEvent *event)
{
    QPixmap pixmap;
    pixmap = QPixmap::grabWidget(this);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    int groupId = _animation->program()->animationGroups().indexOf(_animation->group());
    if(groupId!=-1)
    {
        int animationId = _animation->program()->animationGroups().at(groupId)->animations().indexOf(_animation);
        dataStream
                << QString("UiAnimation")
                << QPoint(event->pos() - this->pos())
                << _animation->program()->id()
                << groupId
                << animationId;

        QMimeData *mimeData = new QMimeData;
        mimeData->setData("application/x-dndanimation", itemData);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap);
        drag->setHotSpot(event->pos());

        this->setEnabled(false);
        this->setProperty("dragged", true);
        this->style()->unpolish(this);
        this->style()->polish(this);

        if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        {
            this->close();
        } else {
            this->show();
            this->setEnabled(true);
            this->setProperty("dragged", false);
            this->style()->unpolish(this);
            this->style()->polish(this);
        }
    }
}

void UiAnimation::changeAnimationGroup(int programId, int groupId)
{
    qDebug() << "UiAnimation::changeAnimationGroup(" << programId << "," << groupId << ")"
             << "sender" << sender();
    emit animationMoved(programId, groupId);
}
