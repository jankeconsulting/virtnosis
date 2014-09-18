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

Domain::Domain(virDomainPtr domain, QObject *parent) :
    QObject(parent),
    m_domain(domain)
{
    virDomainRef(domain);
}

Domain::Domain(const Domain &domain) :
    QObject(),
    m_domain(0)
{
    setParent(domain.parent());
    this->m_domain = domain.m_domain;
    virDomainRef(m_domain);
}

Domain::~Domain()
{
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

    if(virDomainGetState(m_domain, &state, &reason, 0) != -1)
        return state;

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


