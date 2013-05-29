#ifndef MIDIMAPPING_H
#define MIDIMAPPING_H

#include <QObject>
#include <QMap>
#include <QVariant>

class MidiMapping : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString vendor READ vendor WRITE setVendor STORED true)
    Q_PROPERTY(QString product READ product WRITE setProduct STORED true)
    Q_PROPERTY(QMap map READ map STORED true)
public:
    explicit MidiMapping(QObject *parent = 0);
    
    void setVendor(const QString& vendor);
    QString vendor() const { return _vendor; }

    void setProduct(const QString& product);
    QString product() const { return _product; }

    QMap<QString,QVariant> map() const { return _map; }
    void addAssignedMidiControl(const QString &role, const quint8 channel, const quint8 control);

    static MidiMapping *loadFromFile(const QString &filename, QObject *parent = 0);
//    static bool saveToFile(QString filename);

signals:
    
public slots:

private:
    QString _vendor;
    QString _product;
    QMap<QString,QVariant> _map;
    bool _modified;
};

#endif // MIDIMAPPING_H
