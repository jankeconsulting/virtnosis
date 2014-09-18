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
    DomainItem *domain_item;

    QList<Domain *> list = hypervisor->domains();
    QListIterator<Domain *> i(list);
    while (i.hasNext()){

        Domain *domain = i.next();
        domain_item = new DomainItem(domain->name());
        this->appendRow(domain_item);
        QVariant hypervisor_varobj;
        QModelIndex index = this->child(this->rowCount()-1, 0)->index();
        hypervisor_varobj.setValue(*hypervisor);
        QAbstractItemModel *model = this->model();

        QVariant domain_varobj;
        domain_varobj.setValue(*domain);
        model->setData(index, domain_varobj, DomainViewModel::domainDomainRole);
        model->setData(index, hypervisor_varobj, DomainViewModel::domainHypervisorRole);
        model->setData(index, DomainViewModel::typeDomain, DomainViewModel::domainTypeRole);
    }
}

void HypervisorItem::removeDomainsFromItem(Hypervisor *hypervisor)
{

    this->removeRows(0, this->rowCount());
}
