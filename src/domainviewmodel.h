/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINVIEWMODEL_H
#define DOMAINVIEWMODEL_H

#include <QStandardItemModel>
#include "domainitem.h"

class DomainViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DomainViewModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    enum datarole {
                   domainNameRole = Qt::UserRole + 100,
                   domainStateRole = Qt::UserRole + 101,
                   domainIconRole = Qt::UserRole + 102,
                   domainHypervisorRole = Qt::UserRole + 103,
                  };

signals:

public slots:

};

#endif // DOMAINVIEWMODEL_H
