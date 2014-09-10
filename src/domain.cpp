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
    qDebug() << "Domain::Domain: create Domain with m_domain = " << m_domain;
}

Domain::Domain(const Domain &domain) :
    QObject(),
    m_domain(0)
{
    setParent(domain.parent());
    this->m_domain = domain.m_domain;
}

Domain::~Domain()
{
    virDomainFree(m_domain);
}
int Domain::state()
{
    int state;
    int reason;

    qDebug() << "Domain::state: this" << this;
    qDebug() << "Domain::state: m_domain" << m_domain;
    if(virDomainGetState(m_domain, &state, &reason, 0) != -1)
        return state;

    return -1;
}
