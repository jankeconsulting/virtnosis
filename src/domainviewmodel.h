/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINVIEWMODEL_H
#define DOMAINVIEWMODEL_H

#include <QStandardItemModel>

class DomainViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit DomainViewModel(QObject *parent = 0);

signals:

public slots:

};

#endif // DOMAINVIEWMODEL_H
