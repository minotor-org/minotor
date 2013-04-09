#ifndef MINOPROPERTYTEXT_H
#define MINOPROPERTYTEXT_H

#include <QString>

#include "minoproperty.h"

class MinoPropertyText : public MinoProperty
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged STORED true)

public:
    explicit MinoPropertyText(QObject *parent);

    QString text() { return _text; }

public slots:
    void setText(const QString& text);

signals:
    void textChanged(const QString& text);

protected:
    QString _text;

};
#endif // MINOPROPERTYTEXT_H
