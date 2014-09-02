/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisoritem.h"
#include <QDebug>

HypervisorItem::HypervisorItem(Hypervisor *hypervisor) :
    QStandardItem(hypervisor->name()),
    m_hypervisor(hypervisor)
{
    addDomainsFromHypervisor(hypervisor);
}

void HypervisorItem::addDomainsFromHypervisor(Hypervisor *hypervisor)
{
    virDomainPtr *domains;
    int i;
    DomainItem *domain_item;

    /*
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING |
                         VIR_CONNECT_LIST_DOMAINS_PERSISTENT;
    */

    virConnectPtr connection = virConnectOpen(hypervisor->uri().toLatin1().data());

    qDebug() << "VirtnosisWindow::list_domains: connection = " << connection;

    int number_of_domains = virConnectListAllDomains(connection, &domains, 0);
    qDebug() << "VirtnosisWindow::list_domains: number_of_domains = " << number_of_domains;

    if (number_of_domains < 0)
        qDebug() << "VirtnosisWindow::list_domains: error = " << number_of_domains;

    for (i = 0; i < number_of_domains; i++) {

        qDebug() << "VirtnosisWindow::list_domains: domains[" << i << "] = " << domains[i] << "name = " << virDomainGetName(domains[i]);
        domain_item = new DomainItem(virDomainGetName(domains[i]));
        this->appendRow(domain_item);
        virDomainFree(domains[i]);
    }

    free(domains);


}
