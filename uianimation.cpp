#include "uianimation.h"

#include <QLayout>
#include <QSizePolicy>
#include <QStyle>
#include <QCheckBox>
#include <QToolButton>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

#include <QDebug>

#include "uianimationproperty.h"

#include "minoprogram.h"
#include "minoanimationgroup.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QGroupBox(parent),
    _animation(animation)
{
    this->setMinimumWidth(115);
    this->setMaximumWidth(115);

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
    lDescription->setContentsMargins(4,3,4,0);

    QWidget *wLeft = new QWidget(wDescription);
    wLeft->setMinimumSize(12,12);
    wLeft->setMaximumSize(12,12);
    lDescription->addWidget(wLeft);

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

    //Delete button
    QPushButton *pbDelete = new QPushButton(wDescription);
    pbDelete->setFocusPolicy(Qt::NoFocus);
    pbDelete->setIcon(QIcon(":/pictos/close.png"));
    pbDelete->setIconSize(QSize(8,8));
    pbDelete->setMinimumSize(12,12);
    pbDelete->setMaximumSize(12,12);
    connect(pbDelete, SIGNAL(clicked()), _animation, SLOT(deleteLater()));
    lDescription->addWidget(pbDelete);
    lDescription->setAlignment(pbDelete,Qt::AlignTop);

    _wProperties = new QWidget(wContent);
    QVBoxLayout *lProperties = new QVBoxLayout(_wProperties);
    lContent->addWidget(_wProperties);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

    MinoProperties mpl = animation->findChildren<MinoProperty*>();
    const int maxColumnCount = 2;
    int columnCount = 0;

    QWidget *wPropGroup = new QWidget(_wProperties);
    QHBoxLayout *lPropGroup = new QHBoxLayout(wPropGroup);
    for(int i=0; i<mpl.count(); ++i)
    {
        MinoProperty *property = mpl.at(i);
        UiAnimationProperty *uiAnimationProperty = new UiAnimationProperty(property, _wProperties, true);
        uiAnimationProperty->setObjectName("animationproperty");

        if(i==0)
        {
            // Initialise layout
            lProperties->addWidget(wPropGroup);
            lPropGroup->setSpacing(5);
            lPropGroup->setMargin(0);
            lPropGroup->setContentsMargins(0,0,0,0);
        }

        if(columnCount+uiAnimationProperty->columnCount() > maxColumnCount)
        {
            // Add a separator
            QFrame *fSeparator = new QFrame(_wProperties);
            fSeparator->setObjectName("line");
            fSeparator->setFrameShape(QFrame::HLine);
            fSeparator->setFrameShadow(QFrame::Sunken);
            fSeparator->setLineWidth(1);
            lProperties->addWidget(fSeparator);
            // Create a new row
            wPropGroup = new QWidget(_wProperties);
            lProperties->addWidget(wPropGroup);
            lPropGroup = new QHBoxLayout(wPropGroup);
            lPropGroup->setSpacing(5);
            lPropGroup->setMargin(0);
            lPropGroup->setContentsMargins(0,0,0,0);
            columnCount = uiAnimationProperty->columnCount();
        }
        else
        {
            columnCount += uiAnimationProperty->columnCount();
        }
        // Add property there to the right layout
        lPropGroup->addWidget(uiAnimationProperty);
    }
    lContent->addStretch(1);
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
    event->accept();
    QPixmap pixmap;
    pixmap = QPixmap::grabWidget(this);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    const int groupId = _animation->group()->id();
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
    emit animationMoved(programId, groupId);
}
