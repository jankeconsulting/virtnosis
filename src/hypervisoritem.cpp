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
    int state = 0;
    int reason = 0;

    /*
    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING |
                         VIR_CONNECT_LIST_DOMAINS_PERSISTENT;
    */

    virConnectPtr connection = hypervisor->connection();

    qDebug() << "VirtnosisWindow::list_domains: connection = " << connection;

    int number_of_domains = virConnectListAllDomains(connection, &domains, 0);
    qDebug() << "VirtnosisWindow::list_domains: number_of_domains = " << number_of_domains;

    if (number_of_domains < 0)
        qDebug() << "VirtnosisWindow::list_domains: error = " << number_of_domains;

    for (i = 0; i < number_of_domains; i++) {

        qDebug() << "VirtnosisWindow::list_domains: domains[" << i << "] = " << domains[i] << " name = " << virDomainGetName(domains[i]) << " state = " << virDomainIsActive(domains[i]);
        domain_item = new DomainItem(virDomainGetName(domains[i]));
        this->appendRow(domain_item);
        QVariant hypervisor_varobj;
        QModelIndex index = this->child(this->rowCount()-1, 0)->index();
        hypervisor_varobj.setValue(*hypervisor);
        QAbstractItemModel *model = this->model();

        Domain domain = Domain(domains[i]);
        QVariant domain_varobj;
        domain_varobj.setValue(domain);
        model->setData(index, domain_varobj, DomainViewModel::domainDomainRole);
        if(virDomainGetState(domains[i], &state, &reason, 0) != -1)
            model->setData(index, state, DomainViewModel::domainStateRole);
        model->setData(index, hypervisor_varobj, DomainViewModel::domainHypervisorRole);
        qDebug() << "VirtnosisWindow::list_domains:  = " << number_of_domains;

        virDomainFree(domains[i]);
    }

    free(domains);
}
