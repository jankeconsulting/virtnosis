/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "domainviewmodel.h"
#include <QDebug>

/**
 * @brief constructs object of the model
 * @param parent - parent of the object
 */
DomainViewModel::DomainViewModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

/**
 * @brief provides data from the model
 * @param index - index of item requested
 * @param role - role of the data requested
 * @return
 *
 * Reimplemented method that provides the data according to given role
 * from the model
 */
QVariant DomainViewModel::data(const QModelIndex &index, int role) const
{
    if (role == domainStateRole)
    {
        if (indexIsDomain(index))
        {
            Domain item = domain(index);
            return QVariant(item.state());
        } else
        {
            return QVariant();
        }
    }
    if (role == hypervisorConnectedRole)
    {
        if (indexIsHypervisor(index))
        {
            Hypervisor item = hypervisor(index);
            return QVariant(item.alive());
        } else
        {
            return QVariant();
        }
    }
    return QStandardItemModel::data(index, role);
}

/**
 * @brief connects the given hypervisor
 * @param index - index of the hypervisor
 */
void DomainViewModel::connectHypervisor(const QModelIndex &index)
{
    if (indexIsHypervisor(index))
    {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        hypervisorobj.setAutoConnect(true);
        item->addDomainsFromHypervisor(&hypervisorobj);
        setHypervisor(index, &hypervisorobj);
        dataChanged(index,
                    index,
                    QVector<int>(Qt::DisplayRole, hypervisorConnectedRole));
    }
}

/**
 * @brief disconnects the given hypervisor
 * @param index - index of the hypervisor
 */
void DomainViewModel::disconnectHypervisor(const QModelIndex &index)
{
    if (indexIsHypervisor(index))
    {
        HypervisorItem *item = static_cast<HypervisorItem *>(itemFromIndex(index));
        Hypervisor hypervisorobj = hypervisor(index);
        item->removeDomainsFromItem(&hypervisorobj);
        setHypervisor(index, &hypervisorobj);
    }
}

/**
 * @brief provides the hypervisor object
 * @param index - index of the requested hypervisor
 * @return the hypervisor object of the given index
 */
Hypervisor DomainViewModel::hypervisor(const QModelIndex &index) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index, domainHypervisorRole));
}

/**
 * @brief provides the hypervisor object
 * @param row - row of the requested hypervisor
 * @return the hypervisor object in the given row
 */
Hypervisor DomainViewModel::hypervisor(const int row) const
{
    return qvariant_cast<Hypervisor>(QStandardItemModel::data(index(row, 0), domainHypervisorRole));
}

/**
 * @brief sets the hypervisor object
 * @param index - index where the hypervisor is set inside the model
 * @param hypervisor - hypervisor object
 */
void DomainViewModel::setHypervisor(const QModelIndex &index, Hypervisor *hypervisor)
{
    QVariant var;
    var.setValue(*hypervisor);
    setData(index, var, domainHypervisorRole);
    dataChanged(index, index, QVector<int>(hypervisorConnectedRole));
}

/**
 * @brief provides the domain object
 * @param index - index of the requested domain
 * @return the domain of the given index
 */
Domain DomainViewModel::domain(const QModelIndex &index) const
{
    return qvariant_cast<Domain>(QStandardItemModel::data(index, domainDomainRole));
}

/**
 * @brief indicates if item is a hypervisor
 * @param index - index of the item
 * @return true if the item for the given index is a hypervisor, otherwise false
 */
bool DomainViewModel::indexIsHypervisor(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeHypervisor);
}

/**
 * @brief indicates if item is a domain
 * @param index - index of the item
 * @return true if the item for the given index is a domain, otherwise false
 */
bool DomainViewModel::indexIsDomain(const QModelIndex &index) const
{
    return (qvariant_cast<int>(QStandardItemModel::data(index, domainTypeRole)) == typeDomain);
}
