/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domain.h"
#include <QDebug>

/**
 * @brief Domain::Domain
 * @param parent
 *
 * Constructor that creates a domain object. The parameter parent
 * contains the parent object according to the Qt object definition.
 *
 * This constructor creates an "empty" domain, i.e. no reference
 * to a real domain in libvirt is assigned.
 */
Domain::Domain(QObject *parent) :
    QObject(parent),
    m_domain(0)
{

}

/**
 * @brief Domain::Domain
 * @param domain
 * @param version
 * @param parent
 *
 *
 */
Domain::Domain(virDomainPtr domain, ulong version, QObject *parent) :
    QObject(parent),
    m_domain(domain),
    m_libVersion(version)
{
    virDomainRef(domain);
}

/**
 * @brief Domain::Domain
 * @param domain
 */
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

/**
 * @brief Domain::~Domain
 */
Domain::~Domain()
{
    if(m_domain)
        virDomainFree(m_domain);
}

/**
 * @brief Domain::name
 * @return
 */
QString Domain::name()
{
    return virDomainGetName(m_domain);
}

/**
 * @brief Domain::state
 * @return
 */
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

/**
 * @brief Domain::isRunning
 * @return
 */
bool Domain::isRunning()
{
    if(state() == VIR_DOMAIN_RUNNING) {
        return true;
    }

    return false;
}

/**
 * @brief Domain::isPaused
 * @return
 */
bool Domain::isPaused()
{
    if(state() == VIR_DOMAIN_PAUSED) {
        return true;
    }

    return false;
}

/**
 * @brief Domain::start
 * @return
 */
bool Domain::start()
{
//    TODO: check if it can be started
    return (virDomainCreate(m_domain) == 0);
}

/**
 * @brief Domain::reboot
 * @return
 */
bool Domain::reboot()
{
//    TODO: check if it can be rebooted
    return (virDomainReboot(m_domain, 0) == 0);
}

/**
 * @brief Domain::shutdown
 * @return
 */
bool Domain::shutdown()
{
//    TODO: check if it can be stopped
    return (virDomainShutdown(m_domain) == 0);
}

/**
 * @brief Domain::destroy
 * @return
 */
bool Domain::destroy()
{
//    TODO: check if it can be stopped
    return (virDomainDestroy(m_domain) == 0);
}

/**
 * @brief Domain::pause
 * @return
 */
bool Domain::pause()
{
//    TODO: check if it can be paused
    return (virDomainSuspend(m_domain) == 0);
}

/**
 * @brief Domain::resume
 * @return
 */
bool Domain::resume()
{
//    TODO: check if it can be resumed
    return (virDomainResume(m_domain) == 0);
}

/**
 * @brief Domain::info
 * @return
 */
bool Domain::info()
{
    int error = virDomainGetInfo(m_domain, m_info);
    return (error = 0);
}

/**
 * @brief Domain::cpu_count
 * @return
 */
int Domain::cpu_count()
{
    if(info()) {
        return (m_info->nrVirtCpu);
    }
    return 1;
}

/**
 * @brief Domain::memory
 * @return
 */
long Domain::memory()
{
    if(info()) {
        return (m_info->memory);
    }
    return 0;
}

/**
 * @brief Domain::cpustats
 * @return
 */
bool Domain::cpustats()
{
    unsigned int nparams = cpu_count();
    virTypedParameterPtr params = static_cast<virTypedParameterPtr>(calloc(nparams, sizeof(virTypedParameter)));
    int start_cpu = 0;
    unsigned int ncpus = cpu_count();
    unsigned int flags = 0;
    return (virDomainGetCPUStats(m_domain, params, nparams, start_cpu, ncpus, flags) == 0);
}

/**
 * @brief Domain::uri
 * @return
 */
QString Domain::uri()
{
    virConnectPtr conn = virDomainGetConnect(m_domain);
    return QString(virConnectGetURI(conn));
}



