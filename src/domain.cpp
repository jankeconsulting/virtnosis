/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "./domain.h"
#include <QDebug>

/**
 * @brief constructs a domain object
 * @param parent - parent object
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
 * @brief constructs a domain object
 * @param domain - domain pointer in libvirt
 * @param version - version of libvirt
 * @param parent - parent object
 *
 * Constructor that creates a domain object. The parameter parent
 * contains the parent object according to the Qt object definition.
 *
 * This constructor creates a domain object that represents the
 * domain referenced by the domain pointer given in the parameter
 * domain.
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
 * @brief constructs a domain object
 * @param domain - domain object to be copied
 *
 * Constructor that copies an existing domain object into
 * a new one. This constructor is needed in order to declare
 * this class a Qt Meta Object.
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
 * @fn Domain::~Domain
 * @brief destroys the domain object
 *
 * If domain object references a valid domain in libvirt,
 * the corresponding memory will be freed.
 */
Domain::~Domain()
{
    if (m_domain)
    {
        virDomainFree(m_domain);
    }
}

/**
 * @fn Domain::name
 * @brief provides the name of the domain
 * @return name of the domain
 */
QString Domain::name()
{
    return virDomainGetName(m_domain);
}

/**
 * @brief provides the state of the Domain in form of integer
 * @return integer representing state of domain
 *
 * The return value respresents the integer from the enum virDomainState
 * (http://libvirt.org/html/libvirt-libvirt-domain.html#virDomainState)
 *
 * If the correct state cannot be ascertained, -1 is returned.
 */
int Domain::state()
{
    int state;
    int reason;

    if (m_domain)
    {
        if (m_libVersion > 8000)
        {
            if (virDomainGetState(m_domain, &state, &reason, 0) != -1)
            {
                return state;
            }
        } else {
            virDomainInfo info;
            if (virDomainGetInfo(m_domain, &info) != -1)
            {
                return info.state;
            }
        }
    }

    return -1;
}

/**
 * @brief provides indication if domain is running
 * @return true if domain is running otherwise false.
 */
bool Domain::isRunning()
{
    if (state() == VIR_DOMAIN_RUNNING)
    {
        return true;
    }

    return false;
}

/**
 * @brief provides indication if domain is paused
 * @return true if domain is paused, otherwise false.
 */
bool Domain::isPaused()
{
    if (state() == VIR_DOMAIN_PAUSED)
    {
        return true;
    }

    return false;
}

/**
 * @brief starts the domain
 * @return true if the domain started without error, otherwise false
 */
bool Domain::start()
{
// TODO(txwikinger): check if it can be started
    return (virDomainCreate(m_domain) == 0);
}

/**
 * @brief reboots the domain
 * @return true if the domain was rebooted without error, otherwise false
 */
bool Domain::reboot()
{
// TODO(txwikinger): check if it can be rebooted
    return (virDomainReboot(m_domain, 0) == 0);
}

/**
 * @brief shuts down the domain
 * @return true if the domain was shut down without error, otherwise false
 */
bool Domain::shutdown()
{
// TODO(txwikinger): check if it can be stopped
    return (virDomainShutdown(m_domain) == 0);
}

/**
 * @brief forceful shutdown of domain
 * @return true if the domain was shut down without error, otherwise false
 *
 * The domain is sent a sigterm signal followed by a sigkill signal after
 * timeout period without success
 */
bool Domain::destroy()
{
// TODO(txwikinger): check if it can be stopped
    return (virDomainDestroy(m_domain) == 0);
}

/**
 * @brief pauses the domain
 * @return if the domain was paused without error, otherwise false
 */
bool Domain::pause()
{
// TODO(txwikinger): check if it can be paused
    return (virDomainSuspend(m_domain) == 0);
}

/**
 * @brief resumes the domain
 * @return if the domain was resumed without error, otherwise false
 */
bool Domain::resume()
{
// TODO(txwikinger): check if it can be resumed
    return (virDomainResume(m_domain) == 0);
}

/**
 * @brief retrieves information about domain
 * @return if the information was retrieved without error, otherwise false
 */
bool Domain::info()
{
    int error = virDomainGetInfo(m_domain, m_info);
    return (error = 0);
}

/**
 * @brief retrieves the number of virtual cpus that the domain can use
 * @return number of available virtual cpus
 */
int Domain::cpu_count()
{
    if (info())
    {
        return (m_info->nrVirtCpu);
    }
    return 1;
}

/**
 * @brief retrieves the available memory for the domain
 * @return memory in kB
 */
long Domain::memory()
{
    if (info())
    {
        return (m_info->memory);
    }
    return 0;
}

/**
 * @brief retrieves the cpu usage statistics for the domain
 * @return true if the cpu statistic was retrieved without error, otherwise false
 */
bool Domain::cpustats()
{
    unsigned int nparams = cpu_count();
    virTypedParameterPtr params = static_cast<virTypedParameterPtr>(
                calloc(nparams, sizeof(virTypedParameter)));
    int start_cpu = 0;
    unsigned int ncpus = cpu_count();
    unsigned int flags = 0;
    return (virDomainGetCPUStats(m_domain, params, nparams, start_cpu, ncpus, flags) == 0);
}

/**
 * @brief provides the uri used to access the domain
 * @return string containing the uri
 */
QString Domain::uri()
{
    virConnectPtr conn = virDomainGetConnect(m_domain);
    return QString(virConnectGetURI(conn));
}
