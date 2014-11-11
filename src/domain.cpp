/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domain.h"
#include <QDebug>

Domain::Domain(QObject *parent) :
    QObject(parent),
    m_domain(0)
{

}

Domain::Domain(virDomainPtr domain, ulong version, QObject *parent) :
    QObject(parent),
    m_domain(domain),
    m_libVersion(version)
{
    virDomainRef(domain);
}

Domain::Domain(const Domain &domain) :
    QObject(),
    m_domain(0),
    m_libVersion(0)
{
    setParent(domain.parent());
    this->m_domain = domain.m_domain;
    this->m_libVersion = domain.m_libVersion;
    virDomainRef(m_domain);
}

Domain::~Domain()
{
    if(m_domain)
        virDomainFree(m_domain);
}

QString Domain::name()
{
    return virDomainGetName(m_domain);
}

int Domain::state()
{
    int state;
    int reason;

    if(m_domain) {
        if(m_libVersion > 8000) {
            if(virDomainGetState(m_domain, &state, &reason, 0) != -1) {
                return state;
            }
        } else {
            virDomainInfo info;
            if(virDomainGetInfo(m_domain, &info) != -1) {
                return info.state;
            }
        }
    }

    return -1;
}

bool Domain::isRunning()
{
    if(state() == VIR_DOMAIN_RUNNING) {
        return true;
    }

    return false;
}

bool Domain::isPaused()
{
    if(state() == VIR_DOMAIN_PAUSED) {
        return true;
    }

    return false;
}

bool Domain::start()
{
//    TODO: check if it can be started
    return (virDomainCreate(m_domain) == 0);
}

bool Domain::reboot()
{
//    TODO: check if it can be rebooted
    return (virDomainReboot(m_domain, 0) == 0);
}

bool Domain::shutdown()
{
//    TODO: check if it can be stopped
    return (virDomainShutdown(m_domain) == 0);
}

bool Domain::destroy()
{
//    TODO: check if it can be stopped
    return (virDomainDestroy(m_domain) == 0);
}

bool Domain::pause()
{
//    TODO: check if it can be paused
    return (virDomainSuspend(m_domain) == 0);
}

bool Domain::resume()
{
//    TODO: check if it can be resumed
    return (virDomainResume(m_domain) == 0);
}

bool Domain::info()
{
    virDomainInfoPtr info;
    return (virDomainGetInfo(m_domain, info) == 0);
}

int Domain::cpu_count()
{
    return 1;
}

bool Domain::cpustats()
{
    unsigned int nparams = cpu_count();
    virTypedParameterPtr params = static_cast<virTypedParameterPtr>(calloc(nparams, sizeof(virTypedParameter)));
    int start_cpu = 0;
    unsigned int ncpus = cpu_count();
    unsigned int flags = 0;
    return (virDomainGetCPUStats(m_domain, params, nparams, start_cpu, ncpus, flags) == 0);
}

QString Domain::uri()
{
    virConnectPtr conn = virDomainGetConnect(m_domain);
    return QString(virConnectGetURI(conn));
}



