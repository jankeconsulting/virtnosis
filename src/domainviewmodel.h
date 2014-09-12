/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINVIEWMODEL_H
#define DOMAINVIEWMODEL_H

#include <QStandardItemModel>
#include "domainitem.h"
#include "hypervisor.h"
#include "domain.h"
#include <QVariant>

class DomainViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DomainViewModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

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

};

#endif // DOMAINVIEWMODEL_H
