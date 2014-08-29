/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef DOMAINITEM_H
#define DOMAINITEM_H

#include <QStandardItem>
#include "hypervisor.h"

class DomainItem : public QStandardItem
{
//    Q_OBJECT
public:
    explicit DomainItem(const QString &text);
    explicit DomainItem(Hypervisor *hypervisor);

signals:

public slots:

};

#endif // DOMAINITEM_H
