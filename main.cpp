#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // HACK
    qDebug() << "";
    // Load an application style
    QFile styleFile( "style.css" );
    styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    /*
    style = "QWidget { color:#ffffff;}";
    //qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #666666, stop: 0.08 #5a5a5a,stop: 0.14 #4a4a4a, stop: 0.3 #404040, stop: 0.6 #3a3a3a, stop: 1 #2c2c2c);
    //ConfigDialog
    style += "QWidget#panel { background:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0.08 #5a5a5a,stop: 0.14 #4a4a4a, stop: 0.3 #404040, stop: 0.6 #3a3a3a, stop: 1 #2c2c2c);; border-radius:5px; }";
    style += "QWidget#border { background-color:#2c2c2c; border-radius:5px; }";
    style += "QWidget#content { background-color:#2c2c2c;border-radius:5px;}";
    style += "QWidget#toolbarpanel { background-color:#3a3a3a;}";
    style += "QWidget#programbackground {background-color:#3a3a3a;border-radius:5px;}";
    style += "QWidget#scrollbackground {background-color:#151515;border-radius:5px;}";
    style += "QScrollBar:horizontal { border: 1px solid #202020;background: #323232;height: 15px;margin: 0px 20px 0 20px;}";
    style += "QScrollBar:vertical { border: 1px solid #202020;background: #323232;width: 15px;margin: 20px 0 20px 0;}";
    style += "QScrollBar::handle {border: 1px solid #323232;background: #404040;min-width: 20px;}";
    style += "QScrollBar::add-line:horizontal {border: 1px solid #202020; background: #404040;width: 20px;subcontrol-position: right;subcontrol-origin: margin;}";
    style += "QScrollBar::sub-line:horizontal {border: 1px solid #202020;background: #404040;width: 20px;subcontrol-position: left;subcontrol-origin: margin;}";
    style += "QScrollBar:left-arrow:horizontal, QScrollBar::right-arrow:horizontal {border: 1px solid #151515;width: 5px;height: 5px;background: grey;}";
    style += "QScrollArea#scroll {background-color:#3a3a3a;}";
    style +="QGroupBox { background-color:#3a3a3a;border-radius:5px; }";
    style +="QLabel#title {font:bold;font-size:14px;}";
    style +="QLabel#image {background-color:#666666;border-radius:5px;}";
    style +="QFrame#line {background-color:#2c2c2c;}";
    style +="QFrame#view {background-color:#202020;}";
    style +="MainWindow {background-color:#151515;}";
    style +="QCheckBox {background-color:#2c2c2c;}";
    style +="QDockWidget {background-color:#3a3a3a;}";
    style +="QDockWidget::title {padding-left:10px;font:bold;font-size:30px;background-color: #3a3a3a;}";
    style +="QCheckBox#arrow {background-color:transparent;}";
    style +="QCheckBox#arrow::indicator:unchecked {image: url(:/pictos/arrow_closed.png);}";
    style +="QCheckBox#arrow::indicator:checked {image: url(:/pictos/arrow_opened.png);}";
    style +="QPushButton {background-color:#3a3a3a;}";
    style +="QPushButton:pressed {background-color:#e75f00; }";
    style +="QPushButton:checked {background-color:#e75f00; }";
    style +="QDoubleSpinBox {background-color:#2c2c2c;font:bold;}";
    //style +="UiAnimationPicker#scrollcontent {background-color:#454545;}";
*/
    qDebug() << "style" << style;
    a.setStyleSheet( style );

    MainWindow w;

    w.show();

    return a.exec();
}
