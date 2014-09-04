/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISORITEM_H
#define HYPERVISORITEM_H

#include <QStandardItem>
#include "hypervisor.h"
#include "domainitem.h"
#include "domainviewmodel.h"
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"

class HypervisorItem : public QStandardItem
{

public:
    explicit HypervisorItem(Hypervisor *hypervisor);
    void addDomainsFromHypervisor(Hypervisor *hypervisor);    

private:
};

#endif // HYPERVISORITEM_H
