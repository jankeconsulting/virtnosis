/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainviewmodel.h"
#include <QDebug>

/**
 * @brief DomainViewModel::DomainViewModel
 * @param parent
 */
DomainViewModel::DomainViewModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

/**
 * @brief DomainViewModel::data
 * @param index
 * @param role
 * @return
 */
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

/**
 * @brief DomainViewModel::connectHypervisor
 * @param index
 */
void DomainViewModel::connectHypervisor(const QModelIndex &index)
{
    if(indexIsHypervisor(index)) {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        hypervisorobj.setAutoConnect(true);
        item->addDomainsFromHypervisor(&hypervisorobj);
        setHypervisor(index, &hypervisorobj);
        dataChanged(index, index, QVector<int>(Qt::DisplayRole, hypervisorConnectedRole));
    }
}

/**
 * @brief DomainViewModel::disconnectHypervisor
 * @param index
 */
void DomainViewModel::disconnectHypervisor(const QModelIndex &index)
{
    if(indexIsHypervisor(index)) {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        item->removeDomainsFromItem(&hypervisorobj);
        setHypervisor(index, &hypervisorobj);
    }
}

/**
 * @brief DomainViewModel::hypervisor
 * @param index
 * @return
 */
Hypervisor DomainViewModel::hypervisor(const QModelIndex &index) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
}

/**
 * @brief DomainViewModel::hypervisor
 * @param row
 * @return
 */
Hypervisor DomainViewModel::hypervisor(const int row) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index(row, 0), domainHypervisorRole));
}

/**
 * @brief DomainViewModel::setHypervisor
 * @param index
 * @param hypervisor
 */
void DomainViewModel::setHypervisor(const QModelIndex &index, Hypervisor *hypervisor)
{
    QVariant var;
    var.setValue(*hypervisor);
    setData(index, var, domainHypervisorRole);
    dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
}

/**
 * @brief DomainViewModel::domain
 * @param index
 * @return
 */
Domain DomainViewModel::domain(const QModelIndex &index) const
{
    return qvariant_cast<Domain>(QStandardItemModel::data(index, domainDomainRole));
}

/**
 * @brief DomainViewModel::indexIsHypervisor
 * @param index
 * @return
 */
bool DomainViewModel::indexIsHypervisor(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeHypervisor);
}

/**
 * @brief DomainViewModel::indexIsDomain
 * @param index
 * @return
 */
bool DomainViewModel::indexIsDomain(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeDomain);
}
