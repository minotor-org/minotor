#include "ledmatrix.h"

#include <QImage>
#include <QDebug>
#include <QPainter>

LedMatrix::LedMatrix(QObject *parent) :
    QObject(parent),
    _size(24,16),
    _port(NULL),
    _connected(false)
{
    _frame = new QImage(24, 16, QImage::Format_RGB32);
}

LedMatrix::~LedMatrix()
{
    if (_connected) _port->close();
    if (_port) delete _port;
}

bool LedMatrix::openPortByName(QString portName)
{
    _port = new QextSerialPort(portName);
    _port->setBaudRate(BAUD1000000);
    if (_port->open(QIODevice::WriteOnly)){
        qDebug() << "Led matrix connected to:" << this->portName();
        _connected = true;
        emit(connected());
    }
    return _connected;
}

void LedMatrix::closePort()
{
    if (_port) {
        _port->close();
        qDebug() << "Led matrix disconnected.";
        emit(connected(false));
    }
}

bool LedMatrix::isConnected()
{
    return _connected;
}

QString LedMatrix::portName()
{
    if (_port) {
        return _port->portName();
    }
    return "";
}

void LedMatrix::showView(QGraphicsView * view)
{
    _frame->fill(Qt::black);
    QPainter painter(_frame);
    view->render(&painter, QRectF(QRect(0,0,24,16)), view->viewport()->rect(), Qt::IgnoreAspectRatio);
    // qDebug() << "ledmatrix viewport rect:" << view->viewport()->rect() << "scene rect" << view->sceneRect() << "view rect" << view->rect();
    this->show();
}

void LedMatrix::showScene(QGraphicsScene * scene)
{
    this->_frame->fill(Qt::black);
    QPainter painter(this->_frame);
    //painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter, QRect(0,0,24,16), scene->sceneRect(), Qt::IgnoreAspectRatio);
    this->show();
}

void LedMatrix::show()
{
    QRgb *pixels = reinterpret_cast<QRgb*>(_frame->bits());
    for (unsigned int y=0;y<MATRIX_LEDS_Y;y++)
      {
        for (unsigned int x=0;x<MATRIX_LEDS_X;x++) {
          unsigned int id;

          unsigned int x_panel_id = (y%2)
                           ?((MATRIX_PANEL_LEDS_X-1)-(x%MATRIX_PANEL_LEDS_X))
                           :(x%MATRIX_PANEL_LEDS_X);
          unsigned int y_panel_id = (y%MATRIX_PANEL_LEDS_Y);
          unsigned int panel_id = ((y/MATRIX_PANEL_LEDS_Y)%2)
                         ?((MATRIX_PANEL_X-1)-(x/MATRIX_PANEL_LEDS_X)) + ((y/MATRIX_PANEL_LEDS_Y) * MATRIX_PANEL_X)
                         :(x/MATRIX_PANEL_LEDS_X) + ((y/MATRIX_PANEL_LEDS_Y) * MATRIX_PANEL_X);

          id = x_panel_id + (y_panel_id*MATRIX_PANEL_LEDS_X) + (panel_id*MATRIX_PANEL_LEDS);

          const unsigned int r_id = (id * 3) + 1;
          const unsigned int g_id = (id * 3) + 2;
          const unsigned int b_id = (id * 3) + 0;

            //QRgb rgb(this->_frame->pixel(x, y));
            QRgb rgb = pixels[x+(y*MATRIX_LEDS_X)];
            //qDebug() << "x:" << x << "y:" << y << "color RGB" << qRed(rgb) << qGreen(rgb) << qBlue(rgb);

            _framebuffer[r_id] = (qRed(rgb)==0x01)?0:qRed(rgb);
            _framebuffer[g_id] = (qGreen(rgb)==0x01)?0:qGreen(rgb);
            _framebuffer[b_id] = (qBlue(rgb)==0x01)?0:qBlue(rgb);
        }
    }
    if(_connected)
    {
        _port->write((const char*)_framebuffer,(MATRIX_LEDS*3));
        char endFrame = 0x01;
        _port->write(&endFrame,1);
        // qDebug("frame sent");
    }
    emit(updated());
}

