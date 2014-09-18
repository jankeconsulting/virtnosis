/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINVIEWMODEL_H
#define DOMAINVIEWMODEL_H

#include <QStandardItemModel>
#include "domainitem.h"
#include "hypervisoritem.h"
#include "domainitem.h"
#include <QVariant>

class DomainViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DomainViewModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void connectHypervisor(const QModelIndex &index);
    void disconnectHypervisor(const QModelIndex &index);

    enum datarole {
        domainTypeRole = Qt::UserRole + 100,
        domainStateRole = Qt::UserRole + 101,
        domainHypervisorRole = Qt::UserRole + 102,
        domainDomainRole = Qt::UserRole + 103,
        hypervisorConnectedRole = Qt::UserRole + 104,
    };

    enum datatype {
        typeHypervisor,
        typeDomain,
    };

signals:

public slots:

private:
    Hypervisor hypervisor(const QModelIndex &index);
    Domain domain(const QModelIndex &index);
    bool indexIsHypervisor(const QModelIndex &index) const;
    bool indexIsDomain(const QModelIndex &index) const;

};

#endif // DOMAINVIEWMODEL_H
