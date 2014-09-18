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
    /*
    virDomainPtr *domains;
    int i;

    virConnectPtr connection = hypervisor->connection();

//    qDebug() << "VirtnosisWindow::list_domains: connection = " << connection;

    int number_of_domains = virConnectListAllDomains(connection, &domains, 0);
//    qDebug() << "VirtnosisWindow::list_domains: number_of_domains = " << number_of_domains;

    if (number_of_domains < 0)
        qDebug() << "VirtnosisWindow::list_domains: error = " << number_of_domains;

    for (i = 0; i < number_of_domains; i++) {
*/
//        qDebug() << "VirtnosisWindow::list_domains: domains[" << i << "] = " << domains[i] << " name = " << virDomainGetName(domains[i]) << " state = " << virDomainIsActive(domains[i]);

    DomainItem *domain_item;

    QList<Domain *> list = hypervisor->domains();
    QListIterator<Domain *> i(list);
    while (i.hasNext()){

        Domain *domain = i.next();
        qDebug() << "HypervisorItem::addDomainsFromHypervisor: domain = " << domain;
        qDebug() << "HypervisorItem::addDomainsFromHypervisor: domain->name() = " << domain->name();
        domain_item = new DomainItem(domain->name());
        this->appendRow(domain_item);
        QVariant hypervisor_varobj;
        QModelIndex index = this->child(this->rowCount()-1, 0)->index();
        hypervisor_varobj.setValue(*hypervisor);
        QAbstractItemModel *model = this->model();

//        qDebug() << "VirtnosisWindow::list_domains: domain = " << domain;
        QVariant domain_varobj;
        domain_varobj.setValue(*domain);
        model->setData(index, domain_varobj, DomainViewModel::domainDomainRole);
        model->setData(index, hypervisor_varobj, DomainViewModel::domainHypervisorRole);
        model->setData(index, DomainViewModel::typeDomain, DomainViewModel::domainTypeRole);
//        qDebug() << "VirtnosisWindow::list_domains:  = " << number_of_domains;
    }
}

void HypervisorItem::removeDomainsFromItem(Hypervisor *hypervisor)
{

    this->removeRows(0, this->rowCount());
}
