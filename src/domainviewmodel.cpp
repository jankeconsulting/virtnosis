/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainviewmodel.h"
#include <QDebug>

DomainViewModel::DomainViewModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

QVariant DomainViewModel::data(const QModelIndex &index, int role) const
{
    if(role == domainStateRole) {
        if(indexIsDomain(index)) {
            Domain item = domain(index);
            return QVariant(item.state());
        } else {
            return QVariant();
        }
    }
    if(role == hypervisorConnectedRole) {
        if(indexIsHypervisor(index)) {
            Hypervisor item = hypervisor(index);
            return QVariant(item.alive());
        } else {
            return QVariant();
        }
    }
    return QStandardItemModel::data(index, role);
}

void DomainViewModel::connectHypervisor(const QModelIndex &index)
{
    if(indexIsHypervisor(index)) {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        hypervisorobj.setAutoConnect(true);
        item->addDomainsFromHypervisor(&hypervisorobj);
        setHypervisor(index, &hypervisorobj);
    }
}

void DomainViewModel::disconnectHypervisor(const QModelIndex &index)
{
    if(indexIsHypervisor(index)) {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        item->removeDomainsFromItem(&hypervisorobj);
        setHypervisor(index, &hypervisorobj);
    }
}

Hypervisor DomainViewModel::hypervisor(const QModelIndex &index) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
}

Hypervisor DomainViewModel::hypervisor(const int row) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index(row, 0), domainHypervisorRole));
}

void DomainViewModel::setHypervisor(const QModelIndex &index, Hypervisor *hypervisor)
{
    QVariant var;
    var.setValue(*hypervisor);
    setData(index, var, domainHypervisorRole);
    dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
}

Domain DomainViewModel::domain(const QModelIndex &index) const
{
    return qvariant_cast<Domain>(QStandardItemModel::data(index, domainDomainRole));
}

bool DomainViewModel::indexIsHypervisor(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeHypervisor);
}

bool DomainViewModel::indexIsDomain(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeDomain);
}
