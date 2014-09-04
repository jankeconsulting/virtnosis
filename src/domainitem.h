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

public:
    explicit DomainItem(const QString &text, Hypervisor *hypervisor);
    Hypervisor *hypervisor();

private:
    Hypervisor *m_hypervisor;

};

#endif // DOMAINITEM_H
