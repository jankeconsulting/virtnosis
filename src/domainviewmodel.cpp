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
        item->addDomainsFromHypervisor(&hypervisorobj);
        QVariant var;
        var.setValue(hypervisorobj);
        setData(index, var, domainHypervisorRole);
        dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
    }
}

void DomainViewModel::disconnectHypervisor(const QModelIndex &index)
{
    if(indexIsHypervisor(index)) {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        item->removeDomainsFromItem(&hypervisorobj);
        QVariant var;
        var.setValue(hypervisorobj);
        setData(index, var, domainHypervisorRole);
        dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
    }
}

Hypervisor DomainViewModel::hypervisor(const QModelIndex &index) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
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
