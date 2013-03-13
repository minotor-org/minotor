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
#include <QDesktopServices>
#include <QUrl>

#include "uianimationdescription.h"
#include "uianimationpicker.h"
#include "uianimationproperty.h"
#include "uiprogrambank.h"
#include "uiknob.h"

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
    _configDialog = new ConfigDialog(this);

    // External master view
    _externalMasterView = new ExternalMasterView(this);
    _externalMasterView->setVisible(false);

    QVBoxLayout *lCentralWidget = new QVBoxLayout(ui->centralWidget);
    lCentralWidget->setSpacing(5);
    lCentralWidget->setMargin(0);
    lCentralWidget->setContentsMargins(10,0,5,0);

    //Toolbar
    QToolBar *_tToolBar = new QToolBar("Transport",this);
    this->addToolBar(Qt::TopToolBarArea,_tToolBar);

    QWidget *_wToolBar = new QWidget(_tToolBar);
    _tToolBar->addWidget(_wToolBar);
    QHBoxLayout *lToolBar = new QHBoxLayout(_wToolBar);

    QWidget *wToolContainer = new QWidget(this);
    lToolBar->addWidget(wToolContainer);
    wToolContainer->setObjectName("panel");
    QHBoxLayout *lToolContainer = new QHBoxLayout(wToolContainer);
    lToolContainer->setSpacing(0);
    lToolContainer->setMargin(0);
    lToolContainer->setContentsMargins(2,2,2,2);

    QWidget *wBackground = new QWidget(wToolContainer);
    lToolContainer->addWidget(wBackground);
    wBackground->setObjectName("panelcontent");
    QHBoxLayout *lBackground = new QHBoxLayout(wBackground);
    lBackground->setSpacing(10);
    lBackground->setMargin(0);
    lBackground->setContentsMargins(5,2,5,2);
    QButtonGroup *_bgTransport = new QButtonGroup(wBackground);
    //Transport
    QWidget *wTransportButtons = new QWidget(wBackground);
    lBackground->addWidget(wTransportButtons);
    QHBoxLayout *lTransportButtons = new QHBoxLayout(wTransportButtons);
    lTransportButtons->setSpacing(5);
    lTransportButtons->setMargin(0);
    lTransportButtons->setContentsMargins(0,0,0,0);
    QPushButton *pbStart = new QPushButton(wTransportButtons);
    lTransportButtons->addWidget(pbStart);
    _bgTransport->addButton(pbStart);
    pbStart->setCheckable(true);
    pbStart->setIcon(QIcon(":/pictos/play.png"));
    pbStart->setIconSize(QSize(16,16));
    pbStart->setMinimumSize(28,28);
    pbStart->setMaximumSize(28,28);
    connect(pbStart,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiStart()));
    QPushButton *pbStop = new QPushButton(wTransportButtons);
    lTransportButtons->addWidget(pbStop);
    _bgTransport->addButton(pbStop);
    pbStop->setCheckable(true);
    pbStop->setIcon(QIcon(":/pictos/stop.png"));
    pbStop->setIconSize(QSize(16,16));
    pbStop->setMinimumSize(28,28);
    pbStop->setMaximumSize(28,28);

    connect(pbStop,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiStop()));
    QPushButton *pbSync = new QPushButton(wTransportButtons);
    lTransportButtons->addWidget(pbSync);
    pbSync->setIcon(QIcon(":/pictos/sync.png"));
    pbSync->setIconSize(QSize(16,16));
    pbSync->setMinimumSize(28,28);
    pbSync->setMaximumSize(28,28);
    connect(pbSync,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiSync()));


    QWidget *wTempoButtons = new QWidget(wBackground);
    lBackground->addWidget(wTempoButtons);
    QHBoxLayout *lTempoButtons = new QHBoxLayout(wTempoButtons);
    lTempoButtons->setSpacing(5);
    lTempoButtons->setMargin(0);
    lTempoButtons->setContentsMargins(0,0,0,0);
    //BPM
    QDoubleSpinBox *_sbBPM = new QDoubleSpinBox(wTempoButtons);
    _sbBPM->setMinimum(20);
    _sbBPM->setMaximum(300);
    _sbBPM->setSingleStep(0.1);
    _sbBPM->setDecimals(1);
    _sbBPM->setFocusPolicy(Qt::NoFocus);

    lTempoButtons->addWidget(_sbBPM);
    _sbBPM->setValue(_minotor->clockSource()->bpm());
    connect(_minotor->clockSource(), SIGNAL(bpmChanged(double)), _sbBPM, SLOT(setValue(double)));
    connect(_sbBPM,SIGNAL(valueChanged(double)),_minotor->clockSource(),SLOT(setBPM(double)));

    //Tap
    QPushButton *pbTap = new QPushButton(wTempoButtons);
    lTempoButtons->addWidget(pbTap);
    pbTap->setIcon(QIcon(":/pictos/tap.png"));
    pbTap->setIconSize(QSize(20,20));
    pbTap->setMinimumSize(28,28);
    pbTap->setMaximumSize(28,28);
    connect(pbTap,SIGNAL(clicked(bool)),_minotor->clockSource(),SLOT(uiTapOn()));
    //_tToolBar->addSeparator();

    QWidget *wMidiButtons = new QWidget(wBackground);
    lBackground->addWidget(wMidiButtons);
    QHBoxLayout *lMidiButtons = new QHBoxLayout(wMidiButtons);
    lMidiButtons->setSpacing(5);
    lMidiButtons->setMargin(0);
    lMidiButtons->setContentsMargins(0,0,0,0);
    // MIDI toolbar
    QPushButton *pbMidiLearn = new QPushButton(_tToolBar);
    connect(pbMidiLearn,SIGNAL(toggled(bool)),this,SLOT(tbMidiLearnToggled(bool)));
    pbMidiLearn->setText("MIDI learn");
    pbMidiLearn->setCheckable(true);
    lMidiButtons->addWidget(pbMidiLearn);

    lBackground->addStretch();
    //Midi monitors
    QWidget *wMidiMonitor = new QWidget(wBackground);
    lBackground->addWidget(wMidiMonitor);
    QVBoxLayout *lMidiMonitor = new QVBoxLayout(wMidiMonitor);
    lMidiMonitor->setSpacing(0);
    lMidiMonitor->setMargin(0);
    lMidiMonitor->setContentsMargins(0,0,0,0);
    //Midi clock
    QWidget *wMidiClock = new QWidget(wMidiMonitor);
    lMidiMonitor->addWidget(wMidiClock);
    QHBoxLayout *lMidiClock = new QHBoxLayout(wMidiClock);
    lMidiClock->setSpacing(5);
    lMidiClock->setMargin(0);
    lMidiClock->setContentsMargins(0,0,0,0);

    _wClockLed = new QWidget(wMidiClock);
    _wClockLed->setObjectName("offled");
    lMidiClock->addWidget(_wClockLed);
    _wClockLed->setMinimumSize(10,10);
    _wClockLed->setMaximumSize(10,10);

    connect(_minotor,SIGNAL(beatToggled(bool)),this,SLOT(beatToggledReceived(bool)));

    QLabel *tClockLed = new QLabel(wMidiClock);
    lMidiClock->addWidget(tClockLed);
    tClockLed->setText("clock");

    //Midi note
    QWidget *wMidiNote = new QWidget(wMidiMonitor);
    lMidiMonitor->addWidget(wMidiNote);
    QHBoxLayout *lMidiNote = new QHBoxLayout(wMidiNote);
    lMidiNote->setSpacing(5);
    lMidiNote->setMargin(0);
    lMidiNote->setContentsMargins(0,0,0,0);

    QWidget *wNoteLed = new QWidget(wMidiNote);
    wNoteLed->setObjectName("offled");
    lMidiNote->addWidget(wNoteLed);
    wNoteLed->setMinimumSize(10,10);
    wNoteLed->setMaximumSize(10,10);

    QLabel *tNoteLed = new QLabel(wMidiNote);
    lMidiNote->addWidget(tNoteLed);
    tNoteLed->setText("midi note");

    // Viewmode
    QPushButton *pbViewmode = new QPushButton(wBackground);
    lBackground->addWidget(pbViewmode);
    pbViewmode->setText(QString::fromUtf8("η"));
    pbViewmode->setIconSize(QSize(20,20));
    pbViewmode->setMinimumSize(28,28);
    pbViewmode->setMaximumSize(28,28);
    pbViewmode->setCheckable(true);
    connect(pbViewmode,SIGNAL(toggled(bool)),this,SLOT(tbViewmodeToggled(bool)));

    // Fullscreen
    QPushButton *pbFullscreen = new QPushButton(wBackground);
    pbFullscreen->setShortcut(Qt::Key_Escape);
    lBackground->addWidget(pbFullscreen);
    pbFullscreen->setIcon(QIcon(":/pictos/fullscreen.png"));
    pbFullscreen->setIconSize(QSize(20,20));
    pbFullscreen->setMinimumSize(28,28);
    pbFullscreen->setMaximumSize(28,28);
    pbFullscreen->setCheckable(true);
    connect(pbFullscreen,SIGNAL(toggled(bool)),this,SLOT(tbFullScreenToggled(bool)));

    //UiMaster
    _uiMaster = new UiMaster(_minotor->master(), ui->centralWidget);
    lCentralWidget->addWidget(_uiMaster);

    // Programs bank
    UiProgramBank *uiProgramBank = new UiProgramBank(_minotor, ui->centralWidget);
    lCentralWidget->addWidget(uiProgramBank);

    // Default MIDI menu
    _menu.addAction(_actionMidiCapture);

    connect(_uiMaster, SIGNAL(midiLearnToggled(bool)), this, SLOT(tbMidiLearnToggled(bool)));
    _tAnimationToolBar = new QToolBar("Animations",this);
    this->addToolBar(Qt::BottomToolBarArea,_tAnimationToolBar);
    QWidget *_wAnimations = new QWidget(_tAnimationToolBar);
    _tAnimationToolBar->addWidget(_wAnimations);
    QHBoxLayout *lAnimations = new QHBoxLayout(_wAnimations);

    QWidget *wContainer = new QWidget(this);
    lAnimations->addWidget(wContainer);
    wContainer->setObjectName("panel");
    QVBoxLayout *lContainer = new QVBoxLayout(wContainer);
    lContainer->setSpacing(0);
    lContainer->setMargin(0);
    lContainer->setContentsMargins(2,2,2,2);

    QWidget *wContent = new QWidget(wContainer);
    lContainer->addWidget(wContent);
    wContent->setObjectName("panelcontent");
    QVBoxLayout *lContent = new QVBoxLayout(wContent);
    lContent->setSpacing(0);
    lContent->setMargin(0);
    lContent->setContentsMargins(5,5,5,5);

    // Populate animation scrollarea
    QScrollArea *sa =  new QScrollArea();
    sa->setFrameShadow(QFrame::Plain);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    sa->setFocusPolicy(Qt::NoFocus);
    sa->setWidgetResizable(true);
    UiAnimationPicker *ap = new UiAnimationPicker(sa);

    sa->setWidget(ap);
    lContent->addWidget(sa);
    // Set viewmode to live
    tbViewmodeToggled(true);
}

MainWindow::~MainWindow()
{
    delete _actionMidiCapture;
    delete _configDialog;
    delete ui;
}

void MainWindow::on_action_Configuration_triggered()
{
    _configDialog->exec();
}

void MainWindow::on_action_MinotorWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://bitbucket.org/neomilium/minotor/wiki/Home"));
}

void MainWindow::midiCaptureTrigged()
{
    qDebug() << "here:" << QObject::sender()->metaObject()->className();

    if (QString(QObject::sender()->metaObject()->className()) == QString("QAction"))
    {
        QAction *action = ((QAction*)QObject::sender());
        qDebug() << "there:" << action->parent()->metaObject()->className();
        UiKnob * knob = dynamic_cast<UiKnob*>(action->parent());
        if (knob)
        {
            qDebug() << "midiCapture trigged for:" << knob->property()->objectName();
            _minotor->midiMapping()->assignCapturedControlTo(knob->property());
        }
    }
}

void MainWindow::on_sPpqn_valueChanged(int value)
{
    static unsigned int uppqn = 0;
    const unsigned int ppqn = value%24;
    const unsigned int qn = value/24;
    _minotor->dispatchClock(uppqn, value, ppqn, qn);
    uppqn++;
}

void MainWindow::tbMidiLearnToggled(bool checked)
{
    QList<UiMidiProperty*> midiProperties = findChildren<UiMidiProperty*>();
    foreach(UiMidiProperty *midiProperty, midiProperties)
    {
        midiProperty->setMidiLearnMode(checked);
        midiProperty->update();
    }
}

void MainWindow::tbViewmodeToggled(bool on)
{
    _uiMaster->setVisible(on);
    _tAnimationToolBar->setVisible(!on);
}

void MainWindow::tbFullScreenToggled(bool on)
{
    (void)on;
    if(isFullScreen()) {
         this->setWindowState(Qt::WindowMaximized);
      } else {
         this->setWindowState(Qt::WindowFullScreen);
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

void MainWindow::on_pbScene_clicked()
{
    QList<QGraphicsItem*> items = Minotor::minotor()->scene()->items();
    foreach(QGraphicsItem *item, items)
    {
        qDebug() << item;
    }
}

void MainWindow::beatToggledReceived(bool active)
{
    _wClockLed->setObjectName("led");
    _wClockLed->setProperty("active", active);
    _wClockLed->style()->unpolish(_wClockLed);
    _wClockLed->style()->polish(_wClockLed);
}

void MainWindow::on_actionExternal_master_view_toggled(bool on)
{
    _externalMasterView->setVisible(on);
}
