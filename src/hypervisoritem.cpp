/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisoritem.h"
#include <QDebug>

HypervisorItem::HypervisorItem(Hypervisor *hypervisor) :
    QStandardItem(hypervisor->name())
{
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
        QVariant test;
        QModelIndex index = this->child(this->rowCount()-1, 0)->index();
        test.setValue(*hypervisor);
        QAbstractItemModel *model = this->model();
        qDebug() << "VirtnosisWindow::list_domains: item= " << domain_item << ", " << domain_item->model();
        qDebug() << "VirtnosisWindow::list_domains: model = " << model;
        qDebug() << "VirtnosisWindow::list_domains: index = " << index;

        model->setData(index, test, DomainViewModel::domainHypervisorRole);

        virDomainFree(domains[i]);
    }

    free(domains);


}
