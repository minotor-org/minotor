#include "uianimationgroup.h"

#include <QStyle>
#include <QDebug>
#include <QPushButton>
#include <QDrag>

#include "minoprogram.h"
#include "minotor.h"
#include "uianimation.h"
#include "uiprogram.h"
#include "uiprogrambank.h"

UiAnimationGroup::UiAnimationGroup(MinoAnimationGroup *group, QWidget *parent) :
    QGroupBox(parent),
    _group(group),
    _expanded(false)
{
    QString objectName = QString("Ui:%1(%2)")
            .arg(group->metaObject()->className())
            .arg(QString::number((ulong)((void*)group), 16));
    setObjectName(objectName);

    QVBoxLayout *lGroupBox = new QVBoxLayout(this);
    lGroupBox->setSpacing(0);
    lGroupBox->setMargin(0);
    lGroupBox->setContentsMargins(2,2,2,2);

    QWidget *wBorder = new QWidget(this);
    wBorder->setObjectName("group");
    lGroupBox->addWidget(wBorder);
    QVBoxLayout *lBorder = new QVBoxLayout(wBorder);
    lBorder->setSpacing(0);
    lBorder->setMargin(0);
    lBorder->setContentsMargins(1,1,1,1);

    _wContent = new QWidget(wBorder);
    lBorder->addWidget(_wContent);
    QVBoxLayout *lContent = new QVBoxLayout(_wContent);
    lContent->setSpacing(4);
    lContent->setMargin(0);
    lContent->setContentsMargins(0,0,0,0);

    QWidget *wTools = new QWidget(_wContent);
    lContent->addWidget(wTools);
    QHBoxLayout *lTools = new QHBoxLayout(wTools);
    lTools->setSpacing(0);
    lTools->setMargin(0);
    lTools->setContentsMargins(6,4,6,4);

    //Screenshot button
    QPushButton *pbScreenshot = new QPushButton(wTools);
    pbScreenshot->setFocusPolicy(Qt::NoFocus);
    pbScreenshot->setIcon(QIcon(":/pictos/reload.png"));
    pbScreenshot->setIconSize(QSize(12,12));
    pbScreenshot->setMinimumSize(14,14);
    pbScreenshot->setMaximumSize(14,14);
    connect(pbScreenshot, SIGNAL(clicked()), this, SLOT(takeAShot()));
    lTools->addWidget(pbScreenshot);

    lTools->addStretch();

    //Enable button
    _pbEnable = new QPushButton(wTools);
    _pbEnable->setObjectName("enableButton");
    _pbEnable->setFocusPolicy(Qt::NoFocus);
    _pbEnable->setIcon(QIcon(_group->screenshot().scaled(60,40,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    _pbEnable->setIconSize(QSize(58,38));
    _pbEnable->setMinimumSize(60,40);
    _pbEnable->setMaximumSize(60,40);
    _pbEnable->setCheckable(true);
    connect(_pbEnable, SIGNAL(toggled(bool)), _group, SLOT(setEnabled(bool)));
    lTools->addWidget(_pbEnable);

    lTools->addStretch();

    //Delete button
    QPushButton *pbDelete = new QPushButton(wTools);
    pbDelete->setFocusPolicy(Qt::NoFocus);
    pbDelete->setIcon(QIcon(":/pictos/close.png"));
    pbDelete->setIconSize(QSize(12,12));
    pbDelete->setMinimumSize(14,14);
    pbDelete->setMaximumSize(14,14);
    pbDelete->setCheckable(true);
    connect(pbDelete, SIGNAL(toggled(bool)), _group, SLOT(deleteLater()));
    lTools->addWidget(pbDelete);

     _wAnimations = new QWidget(_wContent);
    lContent->addWidget(_wAnimations);
    _lAnimations = new QHBoxLayout(_wAnimations);
    _lAnimations->setSpacing(5);
    _lAnimations->setMargin(0);
    _lAnimations->setContentsMargins(0,0,0,0);

    foreach (MinoAnimation *animation, _group->animations())
    {
        addAnimation(animation);
    }
    this->enable(_group->enabled());
    this->setExpanded(true);
    connect(_group, SIGNAL(enabledChanged(bool)), this, SLOT(enable(bool)));
    connect(_group, SIGNAL(destroyed()), this, SLOT(deleteLater()));

    connect(_group, SIGNAL(animationAdded(QObject*)), this, SLOT(addAnimation(QObject*)));
    connect(_group, SIGNAL(animationMoved(QObject*)), this, SLOT(moveAnimation(QObject*)));
}

template<typename T>
T UiAnimationGroup::findParent()
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

UiAnimationGroup::~UiAnimationGroup()
{
}

void UiAnimationGroup::mousePressEvent(QMouseEvent *event)
{
    event->accept();
    QPixmap pixmap;
    pixmap = QPixmap::grabWidget(this);

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    dataStream << objectName() << QPoint(event->pos() - this->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnd_minoanimationgroup", itemData);

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

void UiAnimationGroup::addAnimation(QObject *animation)
{
    qDebug() << Q_FUNC_INFO
             << "animation:" << animation
             << "this:" << this;

    MinoAnimation* a = qobject_cast<MinoAnimation*>(animation);
    Q_ASSERT(a);
    addAnimation(a, a->id());
}

void UiAnimationGroup::addAnimation(MinoAnimation *animation, int index)
{
    if (index == -1)
        index = _lAnimations->count();
    UiAnimation *uiAnimation = new UiAnimation(animation, _wAnimations);
    uiAnimation->setExpanded(_expanded);
    _lAnimations->insertWidget(index, uiAnimation);
}

void UiAnimationGroup::moveAnimation(QObject *animation)
{
    qDebug() << Q_FUNC_INFO
             << "animation:" << animation
             << "this:" << this;

    // Find program bank from this
    UiProgramBank* uipb = findParent<UiProgramBank*>();
    Q_ASSERT(uipb);

    // Look for corresponding uianimation into bank
    QString objectName = QString("Ui:%1(%2)")
            .arg(animation->metaObject()->className())
            .arg(QString::number((ulong)((void*)animation), 16));
    QList<UiAnimation*> uial = findChildren<UiAnimation*>(objectName);
    Q_ASSERT(uial.count()==1);
    UiAnimation *uia = uial.at(0);

    // Remove uianimation from this uigroup
    _lAnimations->removeWidget(uia);

    MinoAnimation* ma = qobject_cast<MinoAnimation*>(animation);
    Q_ASSERT(ma);

    if(this->group()==ma->group())
    {
        // Animation moved within this group
        insertUiAnimation(uia,uia->animation()->id());
    }
    else
    {
        // Look for corresponding uianimationgroup into bank
        QString objectName = QString("Ui:%1(%2)")
                .arg(ma->group()->metaObject()->className())
                .arg(QString::number((ulong)((void*)ma->group()), 16));
        QList<UiAnimationGroup*> uiagl = uipb->findChildren<UiAnimationGroup*>(objectName);
        Q_ASSERT(uiagl.count()==1);
        UiAnimationGroup *uiag = uiagl.at(0);
        Q_ASSERT(uiag);
        Q_ASSERT(uiag != this);
        uiag->insertUiAnimation(uia, uia->animation()->id());
    }
}

void UiAnimationGroup::insertUiAnimation(UiAnimation *animation, int destId)
{
    _lAnimations->insertWidget(destId, animation);
    animation->setExpanded(_expanded);
    animation->setParent(_wAnimations);
    animation->show();
}

void UiAnimationGroup::setExpanded(bool on)
{
    _expanded = on;
    foreach (UiAnimation *animation,this->findChildren<UiAnimation*>())
    {
        animation->setExpanded(on);
    }
}

void UiAnimationGroup::enable(const bool on)
{
    _pbEnable->setChecked(on);
    this->setProperty("active", on);
    this->style()->unpolish(this);
    this->style()->polish(this);
}

void UiAnimationGroup::takeAShot()
{
    QPixmap *pixmap = Minotor::minotor()->graphicsItemToPixmap(_group->itemGroup());
    _group->setScreenshot(*pixmap);
    _pbEnable->setIcon(QIcon(_group->screenshot().scaled(60,40,Qt::KeepAspectRatio,Qt::SmoothTransformation)));
    delete pixmap;
}
