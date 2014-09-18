/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAIN_H
#define DOMAIN_H

#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"

#include <QObject>

class Domain : public QObject
{
    Q_OBJECT
public:
    explicit Domain(QObject *parent = 0);
    explicit Domain(virDomainPtr domain, QObject *parent = 0);
    Domain(const Domain &domain);
    ~Domain();

    QString name();
    int state();
    bool isRunning();
    bool isPaused();
    bool start();
    bool reboot();
    bool shutdown();
    bool pause();
    bool resume();

signals:

public slots:

private:
    virDomainPtr m_domain;

};

Q_DECLARE_METATYPE(Domain)

#endif // DOMAIN_H
