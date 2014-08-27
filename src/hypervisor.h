/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISOR_H
#define HYPERVISOR_H

#include <QObject>

class Hypervisor : public QObject
{
    Q_OBJECT
public:
    explicit Hypervisor(QObject *parent = 0);
    explicit Hypervisor(QString host, QString user, int port = 22, QString protocol = "qemu+ssh", QString path = "system", QObject *parent = 0);

    QString uri();

signals:

public slots:
    void start();

private:
    QString protocol;
    QString account;
    QString host;
    int port;
    QString path;

};

#endif // HYPERVISOR_H
