#ifndef MINOMASTER_H
#define MINOMASTER_H

#include "minoprogram.h"

class MinoMaster : public QObject
{
    Q_OBJECT
public:
    explicit MinoMaster();
    ~MinoMaster();

    QString name() { return QString("Master"); }
    void setProgram(MinoProgram *program);
    MinoProgram *program() { return _program; }

private:
    MinoProgram *_program;
    bool _shifted;

signals:
    void programChanged();
public slots:
    void setBrightness(qreal value);
    void noteChanged(quint8 channel, quint8 note, bool on, quint8 value);
};

#endif // MINOMASTER_H
