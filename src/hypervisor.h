/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISOR_H
#define HYPERVISOR_H

#include <QObject>
#include <QMetaType>

class Hypervisor : public QObject
{
    Q_OBJECT
public:
    explicit Hypervisor(QObject *parent = 0);
    explicit Hypervisor(const Hypervisor& hypervisor);
    explicit Hypervisor(QString host, QString user, int port = 22, QString protocol = "qemu+ssh", QString path = "system", QObject *parent = 0);
    ~Hypervisor();


    QString uri();
    static QString uri(QString host, QString account, int port, QString protocol = "qemu+ssh", QString path = "system");
    QString name();

signals:

public slots:

private:
    QString protocol;
    QString account;
    QString host;
    int port;
    QString path;

};

// qRegisterMetaType<Hypervisor>("Hypervisor");
Q_DECLARE_METATYPE(Hypervisor)

#endif // HYPERVISOR_H
