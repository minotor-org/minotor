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
#include "minotor.h"

UiAnimation::UiAnimation(MinoAnimation *animation, QWidget *parent) :
    QGroupBox(parent),
    _animation(animation)
{
    QString objectName = QString("Ui:%1(%2)")
            .arg(animation->metaObject()->className())
            .arg(QString::number((ulong)((void*)animation), 16));
    setObjectName(objectName);
    this->setMinimumWidth(175);
    this->setMaximumWidth(175);

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
    wLeft->setMinimumSize(14,14);
    wLeft->setMaximumSize(14,14);
    lDescription->addWidget(wLeft);

    lDescription->addStretch();
    MinoAnimationDescription desc = animation->description();
    _tAnimation = new QLabel(desc.name(),wDescription);
    _tAnimation->setAlignment(Qt::AlignCenter);
    _tAnimation->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    lDescription->addWidget(_tAnimation);
    lDescription->addStretch();

    //Delete button
    QPushButton *pbDelete = new QPushButton(wDescription);
    pbDelete->setToolTip("Delete animation");
    pbDelete->setFocusPolicy(Qt::NoFocus);
    pbDelete->setIcon(QIcon(":/pictos/close.png"));
    pbDelete->setIconSize(QSize(12,12));
    pbDelete->setMinimumSize(14,14);
    pbDelete->setMaximumSize(14,14);
    connect(pbDelete, SIGNAL(clicked()), _animation, SLOT(deleteLater()));
    lDescription->addWidget(pbDelete);
    lDescription->setAlignment(pbDelete,Qt::AlignTop);

    _wProperties = new QWidget(wContent);
    QVBoxLayout *lProperties = new QVBoxLayout(_wProperties);
    lContent->addWidget(_wProperties);

    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);

    MinoProperties mpl = animation->findChildren<MinoProperty*>();
    const int maxColumnCount = 3;
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
            if (columnCount<maxColumnCount)
            {
                lPropGroup->addStretch(1);
            }
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
    lPropGroup->addStretch(1);

    lContent->addStretch(1);
    connect(animation, SIGNAL(destroyed()), this, SLOT(deleteLater()));

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

    dataStream << objectName() << QPoint(event->pos() - this->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnd_minoanimation", itemData);

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
        // Nothing to do...
        this->close();
    } else {
        this->show();
        this->setEnabled(true);
        this->setProperty("dragged", false);
        this->style()->unpolish(this);
        this->style()->polish(this);
    }
}

template <typename T> T UiAnimation::findParent()
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

void UiAnimation::takeAShot()
{
    QPixmap *pixmap = Minotor::minotor()->graphicsItemToPixmap(_animation->graphicItem());
    _tAnimation->setPixmap(pixmap->scaled(60,40,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    delete pixmap;
}
