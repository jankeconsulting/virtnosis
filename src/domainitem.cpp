/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainitem.h"

DomainItem::DomainItem(const QString &text, Hypervisor *hypervisor) :
    QStandardItem(text),
    m_hypervisor(hypervisor)
{

}

Hypervisor *DomainItem::hypervisor()
{
    return m_hypervisor;
}
