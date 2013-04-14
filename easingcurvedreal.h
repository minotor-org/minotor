#ifndef EASINGCURVEDREAL_H
#define EASINGCURVEDREAL_H

#include <QObject>

#include <QEasingCurve>

class EasingCurvedReal : public QObject
{
    Q_OBJECT
public:
    explicit EasingCurvedReal(QObject *parent = 0);
    
    void setStartValue(const qreal start) { _start = start; }
    void setEndValue(const qreal end) { _end = end; }
    void setEasingCurve(const QEasingCurve& ec) { _ec = ec; }
    qreal valueForProgress(const qreal progress) { return ((_ec.valueForProgress(progress) * (_end-_start)) + _start); }

signals:
    
public slots:

private:
    qreal _start;
    qreal _end;
    QEasingCurve _ec;
    
};

#endif // EASINGCURVEDREAL_H
