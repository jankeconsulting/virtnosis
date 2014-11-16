/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisoritem.h"
#include <QDebug>

/**
 * @brief constructs object
 * @param hypervisor to be associated with view item
 */
HypervisorItem::HypervisorItem(Hypervisor *hypervisor) :
    QStandardItem(hypervisor->name())
{
}

/**
 * @brief collects the defined domains from the hypervisor
 * @param hypervisor
 *
 * The domain information is collected through the Hypervisor class
 * and added as children to this object so they display in the view
 * as such
 */
void HypervisorItem::addDomainsFromHypervisor(Hypervisor *hypervisor)
{
    DomainItem *domain_item;

    hypervisor->connection();
    QList<Domain *> list = hypervisor->domains();
    QListIterator<Domain *> i(list);
    while (i.hasNext())
    {

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

/**
 * @brief removes all domains as children from this object
 * @param hypervisor
 *
 * The domains are not removed from the hypervisor itself,
 * but only from the view display
 */
void HypervisorItem::removeDomainsFromItem(Hypervisor *hypervisor)
{
    this->removeRows(0, this->rowCount());
    hypervisor->disconnect();
}
