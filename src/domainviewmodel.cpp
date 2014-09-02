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
    if(role == DomainItem::domainStateRole) {
        DomainItem *item = static_cast<DomainItem *>(this->item(index.row(), index.column()));

        qDebug() << "DomainViewModel::data: index = " << index;
        qDebug() << "DomainViewModel::data: item = " << item;
        if(item)
            qDebug() << "DomainViewModel::data: hypervisor = " << item->hypervisor();

        return QVariant();
    } else {
        return QStandardItemModel::data(index, role);
    }

}
