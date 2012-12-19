#include "ledmatrix.h"

#include <QDebug>
#include <QPainter>

LedMatrix::LedMatrix(QString portName, QObject *parent) :
    QImage(24, 16, QImage::Format_RGB32)
{
    _port = new QextSerialPort(portName);
    _port->setBaudRate(BAUD1000000);
    _port->open(QIODevice::WriteOnly);
}

LedMatrix::~LedMatrix()
{
    _port->close();
    delete _port;
}

void LedMatrix::showView(QGraphicsView * view)
{
    this->fill(Qt::black);
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    view->render(&painter, QRectF(QRect(0,0,24,16)), view->viewport()->rect(), Qt::IgnoreAspectRatio);
    this->show();
}

void LedMatrix::showScene(QGraphicsScene * scene)
{
    this->fill(Qt::black);
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter, QRect(0,0,24,16), scene->sceneRect(), Qt::IgnoreAspectRatio);
    this->show();
}

void LedMatrix::show()
{
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

            QRgb rgb(this->pixel(x, y));

            //qDebug() << "x:" << x << "y:" << y << "color RGB" << qRed(rgb) << qGreen(rgb) << qBlue(rgb);

            _framebuffer[r_id] = (qRed(rgb)==0x01)?0:qRed(rgb);
            _framebuffer[g_id] = (qGreen(rgb)==0x01)?0:qGreen(rgb);
            _framebuffer[b_id] = (qBlue(rgb)==0x01)?0:qBlue(rgb);
        }
    }
    _port->write((const char*)_framebuffer,(MATRIX_LEDS*3));
    char endFrame = 0x01;
    _port->write(&endFrame,1);
    qDebug("frame sent");
}
