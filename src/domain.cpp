/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domain.h"

Domain::Domain(QObject *parent) :
    QObject(parent),
    m_domain(0)
{

}

Domain::Domain(virDomainPtr domain, QObject *parent) :
    QObject(parent),
    m_domain(domain)
{
}

Domain::Domain(const Domain &domain) :
    QObject(),
    m_domain(0)
{
    setParent(domain.parent());
    this->m_domain = domain.m_domain;
}
