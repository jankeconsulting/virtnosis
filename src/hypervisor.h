/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISOR_H
#define HYPERVISOR_H

#include <QObject>
#include <QMetaType>
#include <QString>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "domain.h"

class Hypervisor : public QObject
{
    Q_OBJECT

public:
    explicit Hypervisor(QObject *parent = 0);
    Hypervisor(const Hypervisor& hypervisor);
    explicit Hypervisor(QString host, QString user, int port = 22, QString protocol = "qemu+ssh", QString path = "system", QObject *parent = 0);
    ~Hypervisor();


    QString uri();
    static QString uri(QString host, QString account, int port, QString protocol = "qemu+ssh", QString path = "system");
    QString name();
    virConnectPtr connection();
    void disconnect();
    int alive();
    QList<Domain *> domains();


private:
    QString protocol;
    QString account;
    QString host;
    int port;
    QString path;
    int status;

    virConnectPtr m_connection;
};

Q_DECLARE_METATYPE(Hypervisor)

#endif // HYPERVISOR_H
