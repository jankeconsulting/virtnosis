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
            Domain item = qvariant_cast<Domain>(QStandardItemModel::data(index, domainDomainRole));
            return QVariant(item.state());
        } else {
            return QVariant();
        }
    }
    if(role == hypervisorConnectedRole) {
        if(indexIsHypervisor(index)) {
            Hypervisor hypervisor = qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
            qDebug() << "DomainViewModel::data: hypervisor.alive()" << hypervisor.alive();
            return QVariant(hypervisor.alive());
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
        Hypervisor hypervisor = qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
        item->addDomainsFromHypervisor(&hypervisor);
        QVariant var;
        var.setValue(hypervisor);
        setData(index, var, domainHypervisorRole);
        dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
    }
}

void DomainViewModel::disconnectHypervisor(const QModelIndex &index)
{
    if(indexIsHypervisor(index)) {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisor = qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
        item->removeDomainsFromItem(&hypervisor);
        QVariant var;
        var.setValue(hypervisor);
        setData(index, var, domainHypervisorRole);
        dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
    }
}

Hypervisor DomainViewModel::hypervisor(const QModelIndex &index)
{

}

Domain DomainViewModel::domain(const QModelIndex &index)
{
}

bool DomainViewModel::indexIsHypervisor(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeHypervisor);
}

bool DomainViewModel::indexIsDomain(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeDomain);
}
