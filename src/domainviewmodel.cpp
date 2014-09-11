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
        if(qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeDomain) {
            Domain item = qvariant_cast<Domain>(QStandardItemModel::data(index, domainDomainRole));
            return QVariant(item.state());
        } else {
            return QVariant();
        }
    }
    if(role == hypervisorConnectedRole) {
        if(qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeHypervisor) {
            Hypervisor hypervisor = qvariant_cast<Hypervisor>(QStandardItemModel::data(index, hypervisorConnectedRole));
            return QVariant(hypervisor.alive());
        } else {
            return QVariant();
        }
    }
    return QStandardItemModel::data(index, role);
}
