#ifndef MINOTRIGGER_H
#define MINOTRIGGER_H

#include <QObject>

class MinoTrigger : public QObject
{
    Q_OBJECT
public:
    explicit MinoTrigger(QString role, QObject *parent = 0);
    
    void setStatus(bool on);

    QString role() { return _role; }
signals:
    void toogled(bool on);
    void triggered();

public slots:
    
protected:
    bool _on;
    QString _role;
};

#endif // MINOTRIGGER_H
