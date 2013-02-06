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
    style +="QGroupBox { padding-top:3ex; background-color:#323232;border-radius:3px; }";
    style +="QLabel#title {font:bold;font-size:14px;}";
    style +="QLabel#image {background-color:#666666;border-radius:3px;}";
    style +="QFrame#line {background-color:#2c2c2c;}";
    style +="QFrame#view {background-color:#202020;}";
    style +="MainWindow {background-color:#2c2c2c;}";
    style +="QGroupBox::indicator {width: 20px; height: 20px;}";
    style +="QGroupBox::title {subcontrol-origin: margin;subcontrol-position: top center;padding: 5px;}";
    //style +="QGroupBox::indicator:unchecked {background-color:red;}";

    qDebug() << "style" << style;
    a.setStyleSheet( style );

    MainWindow w;

    w.show();

    return a.exec();
}
