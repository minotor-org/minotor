#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QtCore/QList>
#include <QtCore/QDebug>

#include <QToolBar>
#include <QToolButton>
#include <QLCDNumber>
#include <QSplitter>
#include <QDoubleSpinBox>

#include "uidial.h"
#include "uianimationdescription.h"
#include "uianimationpicker.h"
#include "uianimationproperty.h"
#include "uiprogrambank.h"

#include "minoanimation.h"
#include "minoproperty.h"
#include "minoprogram.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _minotor = Minotor::minotor();

    ui->gvGlobal->setScene(_minotor->scene());

    _actionMidiCapture = new QAction("Assign MIDI control", this);
    connect(_actionMidiCapture, SIGNAL(triggered()), this, SLOT(midiCaptureTrigged()));

    ui->dwDebug->hide();

    // Configuration dialog box
    _configDialog = new ConfigDialog(_minotor, this);

    QVBoxLayout *lCentralWidget = new QVBoxLayout(ui->centralWidget);
    // Programs editor (central)

    //Toolbar
    QToolBar *_tToolBar = this->addToolBar("tools");

    QButtonGroup *_bgTransport = new QButtonGroup(_tToolBar);
    //Transport
    QPushButton *pbStart = new QPushButton(_tToolBar);
    _tToolBar->addWidget(pbStart);
    _bgTransport->addButton(pbStart);
    pbStart->setCheckable(true);
    pbStart->setIcon(QIcon(":/pictos/play.png"));
    pbStart->setIconSize(QSize(16,16));
    pbStart->setMinimumSize(30,30);
    pbStart->setMaximumSize(30,30);
    connect(pbStart,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiStart()));
    //_tToolBar->addSeparator();
    QPushButton *pbStop = new QPushButton(_tToolBar);
    _tToolBar->addWidget(pbStop);
    _bgTransport->addButton(pbStop);
    pbStop->setCheckable(true);
    pbStop->setIcon(QIcon(":/pictos/stop.png"));
    pbStop->setIconSize(QSize(16,16));
    pbStop->setMinimumSize(30,30);
    pbStop->setMaximumSize(30,30);
    //pbStop->setText("Stop");
    connect(pbStop,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiStop()));
    //_tToolBar->addSeparator();
    QPushButton *pbSync = new QPushButton(_tToolBar);
    _tToolBar->addWidget(pbSync);
    pbSync->setIcon(QIcon(":/pictos/sync.png"));
    pbSync->setIconSize(QSize(16,16));
    pbSync->setMinimumSize(30,30);
    pbSync->setMaximumSize(30,30);
    //pbSync->setText("Sync");
    connect(pbSync,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiSync()));
    _tToolBar->addSeparator();
    //BPM
    QDoubleSpinBox *_sbBPM = new QDoubleSpinBox(_tToolBar);
    _sbBPM->setMinimum(20);
    _sbBPM->setMaximum(300);
    _sbBPM->setSingleStep(0.1);
    _sbBPM->setDecimals(1);
    _sbBPM->setFocusPolicy(Qt::NoFocus);

    _tToolBar->addWidget(_sbBPM);
    _sbBPM->setValue(_minotor->clockSource()->bpm());
    connect(_minotor->clockSource(), SIGNAL(bpmChanged(double)), _sbBPM, SLOT(setValue(double)));
    connect(_sbBPM,SIGNAL(valueChanged(double)),_minotor->clockSource(),SLOT(setBPM(double)));

    //Tap
    QPushButton *pbTap = new QPushButton(_tToolBar);
    _tToolBar->addWidget(pbTap);
    pbTap->setIcon(QIcon(":/pictos/tap.png"));
    pbTap->setIconSize(QSize(20,20));
    pbTap->setMinimumSize(30,30);
    pbTap->setMaximumSize(30,30);
    connect(pbTap,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiTapOn()));
    _tToolBar->addSeparator();

    // MIDI toolbar
    QPushButton *pbMidiLearn = new QPushButton(_tToolBar);
    connect(pbMidiLearn,SIGNAL(toggled(bool)),this,SLOT(tbMidiLearnToggled(bool)));
    pbMidiLearn->setText("MIDI learn");
    pbMidiLearn->setCheckable(true);
    _tToolBar->addWidget(pbMidiLearn);

    //UiMaster
    _uiMaster = new UiMaster(_minotor->master(), ui->centralWidget);
    lCentralWidget->addWidget(_uiMaster);
    UiProgramBank *uiProgramBank = new UiProgramBank(_minotor, ui->centralWidget);
    lCentralWidget->addWidget(uiProgramBank);
    // Default MIDI menu
    _menu.addAction(_actionMidiCapture);

    connect(_uiMaster, SIGNAL(midiLearnToggled(bool)), this, SLOT(tbMidiLearnToggled(bool)));

    // Populate animation scrollarea
    QScrollArea *sa =  new QScrollArea();
    sa->setFrameShadow(QFrame::Plain);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setWidgetResizable(true);
    UiAnimationPicker *ap = new UiAnimationPicker(sa);

    sa->setWidget(ap);
    ui->toolbarpanel->layout()->addWidget(sa);
    ui->toolbarpanel->layout()->setSpacing(0);
    ui->toolbarpanel->layout()->setMargin(0);
    ui->toolbarpanel->layout()->setContentsMargins(0,0,0,0);
}

MainWindow::~MainWindow()
{
    delete _actionMidiCapture;
    delete _configDialog;
    delete ui;
}

void MainWindow::on_action_Configuration_triggered()
{
    _configDialog->show();
}

void MainWindow::midiCaptureTrigged()
{
    qDebug() << "here:" << QObject::sender()->metaObject()->className();

    if (QString(QObject::sender()->metaObject()->className()) == QString("QAction"))
    {
        QAction *action = ((QAction*)QObject::sender());
        qDebug() << "there:" << action->parent()->metaObject()->className();
        if (QString(action->parent()->metaObject()->className()) == QString("UiDial"))
        {
            UiDial *dial = ((UiDial*)action->parent());
            qDebug() << "midiCapture trigged for:" << dial->property()->objectName();
            _minotor->midiMapping()->assignCapturedControlTo(dial->property());
        }
    }
}

void MainWindow::customContextMenuRequested(const QPoint &pos)
{
    (void)pos;
    _actionMidiCapture->setParent(qApp->widgetAt(QCursor::pos()));
    _menu.exec(QCursor::pos());
}

void MainWindow::on_sPpqn_valueChanged(int value)
{
    const unsigned int ppqn = value%24;
    const unsigned int qn = value/24;
    _minotor->dispatchClock(value, ppqn, qn);
}

void MainWindow::tbMidiLearnToggled(bool checked)
{
    QList<UiAnimationProperty*> animationProperties = findChildren<UiAnimationProperty*>();
    foreach(UiAnimationProperty *animationProperty, animationProperties)
    {
        animationProperty->setMidiLearnMode(checked);
        animationProperty->update();
    }
}

void MainWindow::on_pbShot_clicked()
{
    static int id = 0;
    QString shotname = "master_";
    shotname.append(QString::number(id++));
    shotname.append(QString(".png"));

    qDebug() << "shotname:" << shotname;
    UiProgramView *masterMonitor = findChild<UiProgramView*>("masterMonitor");
    if(masterMonitor)
    {
        QPixmap p = QPixmap::grabWidget(masterMonitor);
        qDebug() << "saved:" << p.save(shotname);
    }
}
