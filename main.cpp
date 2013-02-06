#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // HACK
    qDebug() << "";
    // Load an application style
    QFile styleFile( ":/style.qss" );
    styleFile.open( QFile::ReadOnly );

    // Apply the loaded stylesheet
    QString style( styleFile.readAll() );
    style = "QWidget { color:#ffffff;}";
    style += "QWidget#panel { background-color:#3a3a3a; }";
    style += "QWidget#content { background-color:#2c2c2c;border-radius:3px;}";
    //style += "QWidget#scrollcontent {background-color:#454545;}";
    style +="QGroupBox { background-color:#323232;border-radius:3px; }";
    style +="QLabel#title {font:bold;font-size:14px;}";
    style +="QLabel#image {background-color:#666666;border-radius:3px;}";
    style +="QFrame#line {background-color:#2c2c2c;}";
    style +="QFrame#view {background-color:#202020;}";
    style +="MainWindow {background-color:#2c2c2c;}";
    style +="QCheckBox {background-color:#2c2c2c;}";
    style +="QDockWidget {background-color:#3a3a3a;}";
    style +="QDockWidget::title {padding-left:10px;font:bold;font-size:30px;background-color: #3a3a3a;}";
    //style +="UiAnimationPicker#scrollcontent {background-color:#454545;}";

    qDebug() << "style" << style;
    a.setStyleSheet( style );

    MainWindow w;

    w.show();

    return a.exec();
}
