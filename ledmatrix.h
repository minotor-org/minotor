#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include <QPixmap>
#include "qextserialport.h"


#define MATRIX_PANEL_X         3
#define MATRIX_PANEL_Y         2
#define MATRIX_PANEL_LEDS_X    8
#define MATRIX_PANEL_LEDS_Y    8
#define MATRIX_PANEL_LEDS      (MATRIX_PANEL_LEDS_X*MATRIX_PANEL_LEDS_Y)
#define MATRIX_LEDS_X          (MATRIX_PANEL_X*MATRIX_PANEL_LEDS_X)
#define MATRIX_LEDS_Y          (MATRIX_PANEL_Y*MATRIX_PANEL_LEDS_Y)
#define MATRIX_LEDS            (MATRIX_LEDS_X*MATRIX_LEDS_Y)

class LedMatrix : public QPixmap
{
   // Q_OBJECT
public:
    explicit LedMatrix(QextSerialPort *port, QObject *parent = 0);
    void show();
private:
    QextSerialPort * _port;
    unsigned char _framebuffer[MATRIX_LEDS*3];
signals:
    
public slots:
    
};

#endif // LEDMATRIX_H
