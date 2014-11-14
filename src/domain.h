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

/**
 * @class Domain
 * @brief The Domain class
 * The Domain class encapsulates the functionality required from libvrt
 * concerning domains (virtual machines)
 */
class Domain : public QObject
{
    Q_OBJECT
public:
    explicit Domain(QObject *parent = 0);
    explicit Domain(virDomainPtr domain, ulong version, QObject *parent = 0);
    Domain(const Domain &domain);
    ~Domain();

    QString name();
    int state();
    bool isRunning();
    bool isPaused();
    bool start();
    bool reboot();
    bool shutdown();
    bool destroy();
    bool pause();
    bool resume();
    bool info();
    int cpu_count();
    long memory();
    bool cpustats();

//    TODO: uri is a temporary shortcut - maybe better to be able to get hypervisor
    QString uri();

signals:

public slots:

private:
    virDomainPtr m_domain;
    ulong m_libVersion;
    virDomainInfoPtr m_info;

};

Q_DECLARE_METATYPE(Domain)

#endif // DOMAIN_H
